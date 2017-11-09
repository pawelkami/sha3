import Tkinter as tk
import tkFileDialog
from functools import partial
import sha3dll
import ctypes
import os
import sys
import threading
import wx
import thread
import threading
import time


class OutputGrabber(object):
    """
    Class used to grab standard output or another stream.
    """
    escape_char = "\b"

    def __init__(self, stream=None, threaded=False):
        self.origstream = stream
        self.threaded = threaded
        if self.origstream is None:
            self.origstream = sys.stdout
        self.origstreamfd = self.origstream.fileno()
        self.capturedtext = ""
        # Create a pipe so the stream can be captured:
        self.pipe_out, self.pipe_in = os.pipe()

    def __enter__(self):
        self.start()
        return self

    def __exit__(self, type, value, traceback):
        self.stop()

    def start(self, panel):
        """
        Start capturing the stream data.
        """
        # Save a copy of the stream:
        self.streamfd = os.dup(self.origstreamfd)
        # Replace the original stream with our write pipe:
        os.dup2(self.pipe_in, self.origstreamfd)
        if self.threaded:
            # Start thread that will read the stream:
            self.workerThread = threading.Thread(target=self.readOutput, args=(panel, ))
            self.workerThread.setDaemon(True)
            self.workerThread.start()
            # Make sure that the thread is running and os.read() has executed:
            time.sleep(0.01)

    def stop(self):
        """
        Stop capturing the stream data and save the text in `capturedtext`.
        """
        # Print the escape character to make the readOutput method stop:
        self.origstream.write(self.escape_char)
        # Flush the stream to make sure all our data goes in before
        # the escape character:
        self.origstream.flush()
        if self.threaded:
            # wait until the thread finishes so we are sure that
            # we have until the last character:
            self.workerThread.join()
        # else:
        #     self.readOutput()
        # Close the pipe:
        os.close(self.pipe_out)
        # Restore the original stream:
        os.dup2(self.streamfd, self.origstreamfd)

    def readOutput(self, panel):
        """
        Read the stream data (one byte at a time)
        and save the text in `capturedtext`.
        """
        panel.append_txt("Log starts\n")
        while True:
            char = os.read(self.pipe_out, 1)
            if not char or self.escape_char in char:
                break
            panel.append_txt(char)

class ExamplePanel(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        self.quote = wx.StaticText(self, label="SHA3 Log :", pos=(10, 10))

        self.logger = wx.TextCtrl(self, pos=(0,40), size=(300,150), style=wx.TE_MULTILINE | wx.TE_READONLY)

    def append_txt(self,txt):
        self.logger.AppendText(txt)

    def onClose(self, event):
        self.close()


def sample_Window():
    try:
        app = wx.App(False)
        frame = wx.Frame(None)
        panel = ExamplePanel(frame)
        frame.Show()
        out = OutputGrabber(sys.stdout, True)
        out.start(panel)
        app.MainLoop()
    except:
	    out.stop()

'''
Class liable to draw window with possibility to specify file to hash
'''
class UI(tk.Frame):

    '''
    Constructor
    '''
    def __init__(self, parent):
        tk.Frame.__init__(self, parent)
        self.initWindow(parent)

    '''
    Method liable to draw all buttons and textboxes
    '''
    def initWindow(self, parent):
        self.sha_opt = tk.IntVar()
        self.sha_opt.set(sha3dll.HashType.SHA3_224)
        self.exported = tk.IntVar()

        self.file = tk.StringVar()

        self.infoText = tk.Label(self, text="info")
        self.infoText.bind("<Button-1>", self.info)
        self.prompt = tk.Label(self, text="Pick file to compute hash SHA3")

        self.path = tk.Entry(self, textvariable=self.file, width=40)
        self.path.config(state='readonly')

        self.exploreBtn = tk.Button(self, text="Select file", command=self.pickFile)
        self.computeBtn = tk.Button(self, text="Compute digest", command=self.compute)

        self.sha224Btn = tk.Radiobutton(self, text="224 bit", variable=self.sha_opt, value=sha3dll.HashType.SHA3_224)
        self.sha256Btn = tk.Radiobutton(self, text="256 bit", variable=self.sha_opt, value=sha3dll.HashType.SHA3_256)
        self.sha384Btn = tk.Radiobutton(self, text="384 bit", variable=self.sha_opt, value=sha3dll.HashType.SHA3_384)
        self.sha512Btn = tk.Radiobutton(self, text="512 bit", variable=self.sha_opt, value=sha3dll.HashType.SHA3_512)

        # lay the widgets out on the screen.
        tk.Label(self, text="").grid(row=0, column=0, columnspan=6)

        self.prompt.grid(row=1, column=1, columnspan=4)
        self.infoText.grid(row=1, column=6)

        tk.Label(self, text="").grid(row=2, column=0, columnspan=6)

        self.path.grid(row=3, column=1, columnspan=6, padx=5, sticky=tk.W)
        self.exploreBtn.grid(row=3, column=6)

        tk.Label(self, text="").grid(row=5, column=0, columnspan=6)

        self.sha224Btn.grid(row=6, column=2)
        self.sha256Btn.grid(row=6, column=3)
        self.sha384Btn.grid(row=6, column=4)
        self.sha512Btn.grid(row=6, column=5)
        self.computeBtn.grid(row=6, column=6)


    '''
    Method draws window where user could pick file
    '''
    def pickFile(self):
        filename = tkFileDialog.askopenfilename()
        self.file.set(filename)


    '''
    Method is liable to print information about program
    '''
    def info(self, event):
        dialog = tk.Toplevel()
        ws = root.winfo_screenwidth()
        hs = root.winfo_screenheight()
        w = 300
        h = 100
        dialog.geometry("%dx%d+%d+%d" % (w, h, ws / 2 - w / 2, hs / 2 - h / 2))
        dialog.title('Info')
        toDisplay = """Application computing value of SHA3 hash of given file"""
        msg = tk.Message(dialog, text=toDisplay, width=220)
        msg.pack()
        tk.Label(dialog, text="").pack()
        button = tk.Button(dialog, text="  OK  ", command=dialog.destroy)
        button.pack()

    '''
    Print message box with hash value
    '''
    def printResult(self, hash):
        dialog = tk.Toplevel()
        ws = root.winfo_screenwidth()
        hs = root.winfo_screenheight()
        w = 280
        h = 130
        dialog.geometry("%dx%d+%d+%d" % (w, h, ws / 2 - w / 2, hs / 2 - h / 2))
        dialog.title('Result')
        toDisplay = str(hash)
        msg = tk.Message(dialog, text=toDisplay, width=220)
        msg.pack()
        tk.Label(dialog, text="").pack()
        button = tk.Button(dialog, text="  OK  ", command=dialog.destroy)
        button.pack()

    '''
    Method is liable to start appropriate algorithm SHA3 version
    '''
    def compute(self):
        hashType = sha3dll.HashType(self.sha_opt.get())

        hashCtx = sha3dll.HashCtx()
        hashCtx.setHashAlgo(hashType)
        hash = hashCtx.computeHash(str(self.file.get()))
        self.printResult(hash)


if __name__ == "__main__":
    t = threading.Thread(target=sample_Window)
    t.start()

    root = tk.Tk()
    root.wm_title('SHA3 Algorithm')
    ws = root.winfo_screenwidth()
    hs = root.winfo_screenheight()
    w = 350
    h = 150
    root.geometry("%dx%d+%d+%d" % (w, h, ws / 2 - w / 2, hs / 2 - h / 2))
    UI(root, ).pack(fill="both", expand=True)
    root.mainloop()

    t.join()
