
from __future__ import division

import time
import colorsys
# import matplotlib.pyplot as plt
# import matplotlib.animation as animation
# import numpy as np

from Adafruit_TCS34725 import TCS34725


class Colorimeter(object):
    """Collects and analyzes data from colorimeters"""

    def __init__(self):

        self.CRGB = {}

        self.white_CRGB = [1, 1, 1, 1]
        self.white_HLS = [0, 0, 0]

        self.red_true = [1, 255, 0, 0]
        self.orange_true = [1, 255, 128, 0]
        self.yellow_true = [1, 255, 255, 0]
        self.lightgreen_true = [1, 128, 255, 0]
        self.green_true = [1, 0, 255, 0]
        self.bluegreen_true = [1, 0, 255, 128]
        self.lightblue_true = [1, 0, 255, 255]
        self.blue_true = [1, 0, 128, 255]
        self.darkblue_true = [1, 0, 0, 255]
        self.bluepurple_true = [1, 128, 0, 255]
        self.purple_true = [1, 255, 0, 255]
        self.redpurple_true = [1, 255, 0, 128]
        self.grey_true = [1, 128, 128, 128]

        self.Rw = 0
        self.Gw = 0
        self.Bw = 0
        self.Hw = 0
        self.Lw = 0
        self.Sw = 0

        # not used, but in case
        self.mpl_symbols = ["x", "o", "D", "^", "<", ">", "v", "h", "s"]

    @staticmethod
    def normalize_triple(A, B, C):
        """Normalize triples of color system ABC to a range from [0.0...1.0]
        relative to the sum of A+B+C
        """
        M = max([A, B, C])
        A = A / M
        B = B / M
        C = C / M
        return A, B, C

    def set_white(self):
        if type(self.white_CRGB) == type(list()):
            A = np.array([self.white_CRGB])
        elif type(self.white_CRGB) == type(np.array([])) and self.white_CRGB.ndim == 1:
            A = np.reshape(self.white_CRGB, (len(self.white_CRGB) / 4, 4))
        else:
            A = self.white_CRGB

        self.Rw = np.mean(A[:, 1])
        self.Gw = np.mean(A[:, 2])
        self.Bw = np.mean(A[:, 3])
        self.Hw, self.Lw, self.Sw = colorsys.rgb_to_hls(self.Rw, self.Gw, self.Bw)

    def white_correct_RGB(self, R, G, B):
        R /= self.Rw
        G /= self.Gw
        B /= self.Bw
        return R, G, B

    def CRGB_HLS(self, CRGB, units="1", white_correct=False):
        """Hue, Luminance, Saturation (HLS) from RGB.
        
        Parameters
        ----------
        C, R, G, B : float, channel counts in range [0.0...1.0]
        units : str, "1", "radians", "degrees" for respective conversion
            of H channel.  Default is "1" representing one complete
            revolution of the Hue Circle where  1 = 2*pi = 360 degrees.
        
        Returns
        -------
        H : float, Hue
        S : float, Saturation
        L : float, Luminance
        
        """
        if type(CRGB) == type(list()):
            A = np.array([CRGB])
        elif type(CRGB) == type(np.array([])) and CRGB.ndim == 1:
            A = np.reshape(CRGB, (len(CRGB) / 4, 4))
        else:
            A = CRGB

        R = np.mean(A[:, 1])
        G = np.mean(A[:, 2])
        B = np.mean(A[:, 3])

        if white_correct:
            R, G, B = self.white_correct_RGB(R, G, B)

        H, L, S = colorsys.rgb_to_hls(R, G, B)

        if white_correct:
            H -= self.Hw

        k = {"1": 1, "radians": 2 * np.pi, "degrees": 360}

        H *= k[units]

        return R, G, B, H, L, S

    def i2c_plot(self, rlim=1.5):

        """Plot in realtime color readings

        Parameters
        ----------
        decimation, float, for screen plotting, delay between readings in seconds

        Returns
        -------

        """

        # Initialize the TCS34725 and use default integration time and gain
        tcs = TCS34725(integrationTime=0xEB, gain=0x01)
        tcs.setInterrupt(False)

        fig = plt.figure()
        ax = plt.axes(polar=True)
        ax.set_rmax(rlim)

        # white center
        R, G, B, H, L, S = self.CRGB_HLS(self.white_CRGB, units="1", white_correct=False)
        ax.scatter(H - self.white_HLS[0], self.white_HLS[2], s=100, marker="v", color='black')

        # ideal color locations
        A_array = [self.red_true,
                   self.orange_true,
                   self.yellow_true,
                   self.lightgreen_true,
                   self.green_true,
                   self.bluegreen_true,
                   self.lightblue_true,
                   self.blue_true,
                   self.darkblue_true,
                   self.bluepurple_true,
                   self.purple_true,
                   self.redpurple_true
                   ]

        for n in range(0, len(A_array)):
            R, G, B, H, L, S = self.CRGB_HLS(A_array[n], units="radians", white_correct=False)
            cc = self.normalize_triple(R, G, B)
            S = 0.4
            ax.scatter(H, S, marker="o", s=100, color=cc)

        # plot to update
        # Returns a tuple of line objects, thus the comma
        p1, = ax.plot(None, None, linestyle='None', marker="o", markersize=20, color="red")

        def init_run():
            p1.set_data(None, None)
            return p1,

        def run(_n, p):
            """Function called by matplotlib.animation.FuncAnimation
            """
            data = tcs.getRawData()
            data = [float(data['c']), float(data['r']), float(data['g']), float(data['b'])]
            _R, _G, _B, _H, _L, _S = self.CRGB_HLS(data, units="radians", white_correct=True)
            p.set_data(_H, _S)
            p.set_markerfacecolor(self.normalize_triple(_R, _G, _B))
            return p,

        ani = animation.FuncAnimation(fig=fig,
                                      func=run,
                                      init_func=init_run,
                                      fargs=(p1,),
                                      frames=20,
                                      blit=True,
                                      interval=10,
                                      repeat=True)

        plt.show()

if __name__ == '__main__':
    AF_olympic_2_white = np.array([11219, 4001, 4114, 2606,
                                   11219, 4001, 4114, 2606,
                                   11219, 4001, 4114, 2606,
                                   11221, 4002, 4114, 2607,
                                   11220, 4002, 4114, 2606])

    c = Colorimeter()
    # commenting this out get the ideal colors to be centered correctly
    c.white_CRGB = AF_olympic_2_white
    c.set_white()
    c.i2c_plot(rlim=1.5)
