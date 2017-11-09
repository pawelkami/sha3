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

# #!/usr/bin/python
# #-*- coding: utf-8 -*-
#
# import Tkinter as tk
# import tkFileDialog
# from functools import partial
# import sha3dll
#
# import os
# import sys
# from contextlib import contextmanager
# import atexit
#
# TEMP_LOG_FILENAME = '.~tmplog.tmp'
#
# def fileno(file_or_fd):
#     fd = getattr(file_or_fd, 'fileno', lambda: file_or_fd)()
#     if not isinstance(fd, int):
#         raise ValueError("Expected a file (`.fileno()`) or a file descriptor")
#     return fd
#
# @contextmanager
# def stdout_redirected(to=os.devnull, stdout=None):
#     if stdout is None:
#        stdout = sys.stdout
#
#     stdout_fd = fileno(stdout)
#     # copy stdout_fd before it is overwritten
#     #NOTE: `copied` is inheritable on Windows when duplicating a standard stream
#     with os.fdopen(os.dup(stdout_fd), 'wb') as copied:
#         stdout.flush()  # flush library buffers that dup2 knows nothing about
#         try:
#             os.dup2(fileno(to), stdout_fd)  # $ exec >&to
#         except ValueError:  # filename
#             with open(to, 'wb') as to_file:
#                 os.dup2(to_file.fileno(), stdout_fd)  # $ exec > to
#         try:
#             yield stdout # allow code to be run with the redirected stdout
#         finally:
#             # restore stdout to its previous value
#             #NOTE: dup2 makes stdout_fd inheritable unconditionally
#             stdout.flush()
#             os.dup2(copied.fileno(), stdout_fd)  # $ exec >&copied
#
# '''
# Class liable to draw window with possibility to specify file to hash
# '''
# class UI(tk.Frame):
#
#     '''
#     Constructor
#     '''
#     def __init__(self, parent):
#         tk.Frame.__init__(self, parent)
#         self.parent = parent
#         self.initWindow(parent)
#
#     '''
#     Method liable to draw all buttons and textboxes
#     '''
#     def initWindow(self, parent):
#         self.sha_opt = tk.IntVar()
#         self.sha_opt.set(sha3dll.HashType.SHA3_224)
#         self.exported = tk.IntVar()
#
#         self.file = tk.StringVar()
#
#         self.infoText = tk.Label(self, text="info")
#         self.infoText.bind("<Button-1>", self.info)
#         self.prompt = tk.Label(self, text="Pick file to compute hash SHA3")
#
#         self.path = tk.Entry(self, textvariable=self.file)
#         self.path.config(state='readonly', width=80)
#
#         self.exploreBtn = tk.Button(self, text="Select file", width=15, command=self.pickFile)
#         self.computeBtn = tk.Button(self, text="Compute digest", width=15, command=self.compute)
#
#         self.sha224Btn = tk.Radiobutton(self, text="224 bit", variable=self.sha_opt, value=sha3dll.HashType.SHA3_224)
#         self.sha256Btn = tk.Radiobutton(self, text="256 bit", variable=self.sha_opt, value=sha3dll.HashType.SHA3_256)
#         self.sha384Btn = tk.Radiobutton(self, text="384 bit", variable=self.sha_opt, value=sha3dll.HashType.SHA3_384)
#         self.sha512Btn = tk.Radiobutton(self, text="512 bit", variable=self.sha_opt, value=sha3dll.HashType.SHA3_512)
#
#         self.logText = tk.Text(self, state="disabled")
#
#         self.scroll = tk.Scrollbar(self.parent)
#
#         self.logText.focus_set()
#         self.scroll.pack(side=tk.RIGHT, fill=tk.Y)
#         self.logText.pack(side=tk.LEFT, fill=tk.Y)
#         self.scroll.config(command=self.logText.yview)
#         self.logText.config(yscrollcommand=self.scroll.set)
#
#         # lay the widgets out on the screen.
#         tk.Label(self, text="").grid(row=0, column=0, columnspan=6)
#
#         self.prompt.grid(row=1, column=1, columnspan=4)
#         self.infoText.grid(row=1, column=6)
#
#         tk.Label(self, text="").grid(row=2, column=0, columnspan=6)
#
#         self.exploreBtn.grid(row=3, column=6)
#         self.path.grid(row=3, column=1, columnspan=5, padx=5, sticky=tk.W)
#
#         tk.Label(self, text="").grid(row=5, column=0, columnspan=6)
#
#         self.sha224Btn.grid(row=6, column=1)
#         self.sha256Btn.grid(row=6, column=2)
#         self.sha384Btn.grid(row=6, column=3)
#         self.sha512Btn.grid(row=6, column=4)
#         self.computeBtn.grid(row=6, column=6)
#
#         self.logText.grid(row=7, column=1, rowspan=2, columnspan=6)
#
#         open(TEMP_LOG_FILENAME, 'w').close()
#         self.logfileChangedTime = os.stat(TEMP_LOG_FILENAME).st_mtime
#
#         self.updateLogs()
#
#     def updateLogs(self):
#         try:
#             actualModifiedTime = os.stat(TEMP_LOG_FILENAME).st_mtime
#             if self.logfileChangedTime != actualModifiedTime:   # file with logs changed, so we can update Text
#                 self.logfileChangedTime = actualModifiedTime    # update modification time
#
#                 with open(TEMP_LOG_FILENAME) as f:
#                     self.logText.configure(state="normal")
#                     self.logText.delete(1.0, tk.END)
#                     str = ""
#                     for i, line in enumerate(f):
#                         str += ('{}. {}\n'.format(i + 1, line.strip()))
#                     self.logText.insert(tk.INSERT, str)
#                     self.logText.see(tk.END)
#                     self.logText.configure(state="disabled")
#         except:
#             pass
#
#         self.parent.after(700, self.updateLogs)
#
#     # method which begins mouse waiting cursor
#     def beginWaitCursor(self):
#         self.parent.config(cursor="wait")
#         self.parent.update()
#
#     # method which stops 'waiting' mouse cursor
#     def endWaitCursor(self):
#         self.parent.config(cursor="")
#         self.parent.update()
#
#     '''
#     Method draws window where user could pick file
#     '''
#     def pickFile(self):
#         filename = tkFileDialog.askopenfilename()
#         self.file.set(filename)
#
#
#     '''
#     Method is liable to print information about program
#     '''
#     def info(self, event):
#         dialog = tk.Toplevel()
#         ws = root.winfo_screenwidth()
#         hs = root.winfo_screenheight()
#         w = 300
#         h = 100
#         dialog.geometry("%dx%d+%d+%d" % (w, h, ws / 2 - w / 2, hs / 2 - h / 2))
#         dialog.title('Info')
#         toDisplay = """Application computing value of SHA3 hash of given file"""
#         msg = tk.Message(dialog, text=toDisplay, width=220)
#         msg.pack()
#         tk.Label(dialog, text="").pack()
#         button = tk.Button(dialog, text="  OK  ", command=dialog.destroy)
#         button.pack()
#
#     '''
#     Print message box with hash value
#     '''
#     def printResult(self, hash):
#         dialog = tk.Toplevel()
#         ws = root.winfo_screenwidth()
#         hs = root.winfo_screenheight()
#         w = 280
#         h = 130
#         dialog.geometry("%dx%d+%d+%d" % (w, h, ws / 2 - w / 2, hs / 2 - h / 2))
#         dialog.title('Result')
#         toDisplay = str(hash)
#         msg = tk.Message(dialog, text=toDisplay, width=220)
#         msg.pack()
#         tk.Label(dialog, text="").pack()
#         button = tk.Button(dialog, text="  OK  ", command=dialog.destroy)
#         button.pack()
#
#     '''
#     Method is liable to start appropriate algorithm SHA3 version
#     '''
#     def compute(self):
#         open(TEMP_LOG_FILENAME, 'w').close()
#
#         hashType = sha3dll.HashType(self.sha_opt.get())
#
#         hashCtx = sha3dll.HashCtx()
#         hashCtx.setHashAlgo(hashType)
#
#         self.beginWaitCursor()
#         with open(TEMP_LOG_FILENAME,"w") as f, stdout_redirected(f):
#             hash = hashCtx.computeHash(str(self.file.get()))
#
#         self.endWaitCursor()
#
#         self.printResult(hash)
#
# def deleteTempFiles():
#     os.remove(TEMP_LOG_FILENAME)
#
#
# if __name__ == "__main__":
#     atexit.register(deleteTempFiles)
#     root = tk.Tk()
#     root.wm_title('SHA3 Algorithm')
#     root.resizable(width=False, height=False)
#     ws = root.winfo_screenwidth()
#     hs = root.winfo_screenheight()
#     w = 350
#     h = 150
#     # root.geometry("%dx%d+%d+%d" % (w, h, ws / 2 - w / 2, hs / 2 - h / 2))
#     UI(root, ).pack(fill="both", expand=True)
#     root.mainloop()
