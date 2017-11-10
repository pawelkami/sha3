import Tkinter as tk
import tkFileDialog
import sha3dll

import os
import sys
from contextlib import contextmanager
import atexit

TEMP_LOG_FILENAME = '.~tmplog.tmp'
logfileChangedTime = 0

def fileno(file_or_fd):
    fd = getattr(file_or_fd, 'fileno', lambda: file_or_fd)()
    if not isinstance(fd, int):
        raise ValueError("Expected a file (`.fileno()`) or a file descriptor")
    return fd

@contextmanager
def stdout_redirected(to=os.devnull, stdout=None):
    if stdout is None:
       stdout = sys.stdout

    stdout_fd = fileno(stdout)
    # copy stdout_fd before it is overwritten
    #NOTE: `copied` is inheritable on Windows when duplicating a standard stream
    with os.fdopen(os.dup(stdout_fd), 'wb') as copied:
        stdout.flush()  # flush library buffers that dup2 knows nothing about
        try:
            os.dup2(fileno(to), stdout_fd)  # $ exec >&to
        except ValueError:  # filename
            with open(to, 'wb') as to_file:
                os.dup2(to_file.fileno(), stdout_fd)  # $ exec > to
        try:
            yield stdout # allow code to be run with the redirected stdout
        finally:
            # restore stdout to its previous value
            #NOTE: dup2 makes stdout_fd inheritable unconditionally
            stdout.flush()
            os.dup2(copied.fileno(), stdout_fd)  # $ exec >&copied

class LogDialog:
    def __init__(self, parent):
        top = self.top = tk.Toplevel(parent)

        tk.Label(top, text="SHA3 Log:").pack()

        self.logText = tk.Text(top, state="disabled")
        self.logText.pack(padx=5)

        self.scroll = tk.Scrollbar(self.top)

        self.logText.focus_set()
        self.scroll.pack(side=tk.RIGHT, fill=tk.Y)
        self.logText.pack(side=tk.LEFT, fill=tk.Y)
        self.scroll.config(command=self.logText.yview)
        self.logText.config(yscrollcommand=self.scroll.set)

        self.top.resizable(width=False, height=False)

        self.updateLogs()

    def close(self):
        self.top.destroy()

    def updateLogs(self):
        global logfileChangedTime
        try:
            actualModifiedTime = os.stat(TEMP_LOG_FILENAME).st_mtime
            if logfileChangedTime != actualModifiedTime:  # file with logs changed, so we can update Text
                logfileChangedTime = actualModifiedTime  # update modification time

                with open(TEMP_LOG_FILENAME) as f:
                    self.logText.configure(state="normal")
                    self.logText.delete(1.0, tk.END)
                    str = ""

                    for i, line in enumerate(f):
                        str += ('{}. {}\n'.format(i + 1, line.strip()))
                    self.logText.insert(tk.INSERT, str)
                    self.logText.see(tk.END)
                    self.logText.configure(state="disabled")
        except:
            pass

        self.top.after(700, self.updateLogs)


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
        self.parent = parent
        self.sha_opt = tk.IntVar()
        self.sha_opt.set(sha3dll.HashType.SHA3_224)
        self.exported = tk.IntVar()

        self.file = tk.StringVar()

        self.infoText = tk.Label(self, text="Logs")
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

        self.logDlg = None

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
        if self.logDlg is not None:
            self.logDlg.close()
        self.logDlg = None
        self.logDlg = LogDialog(self.parent)

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
        open(TEMP_LOG_FILENAME, 'w').close()

        hash = ""
        with open(TEMP_LOG_FILENAME, "w") as f, stdout_redirected(f):
            hashCtx = sha3dll.HashCtx()
            hashType = sha3dll.HashType(self.sha_opt.get())
            hashCtx.setHashAlgo(hashType)

            self.beginWaitCursor()
            hash = hashCtx.computeHash(str(self.file.get()))
            self.endWaitCursor()

        self.printResult(hash)



    # method which begins mouse waiting cursor
    def beginWaitCursor(self):
        self.parent.config(cursor="wait")
        self.parent.update()

    # method which stops 'waiting' mouse cursor
    def endWaitCursor(self):
        self.parent.config(cursor="")
        self.parent.update()

def deleteTempFiles():
    os.remove(TEMP_LOG_FILENAME)

if __name__ == "__main__":
    open(TEMP_LOG_FILENAME, 'w').close()
    atexit.register(deleteTempFiles)

    root = tk.Tk()
    root.wm_title('SHA3 Algorithm')
    ws = root.winfo_screenwidth()
    hs = root.winfo_screenheight()
    w = 350
    h = 150
    root.geometry("%dx%d+%d+%d" % (w, h, ws / 2 - w / 2, hs / 2 - h / 2))
    root.resizable(width=False, height=False)
    UI(root, ).pack(fill="both", expand=True)
    root.mainloop()
