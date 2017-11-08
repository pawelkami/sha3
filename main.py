import Tkinter as tk
import tkFileDialog
from functools import partial
import sha3dll

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
        # print(self.file.get())
        # print(self.sha_opt.get())
        hashType = sha3dll.HashType(self.sha_opt.get())
		
        hashCtx = sha3dll.HashCtx()
        hashCtx.setHashAlgo(hashType)		
        hash = hashCtx.computeHash(str(self.file.get()))
        self.printResult(hash)

if __name__ == "__main__":
    root = tk.Tk()
    root.wm_title('SHA3 Algorithm')
    ws = root.winfo_screenwidth()
    hs = root.winfo_screenheight()
    w = 350
    h = 150
    root.geometry("%dx%d+%d+%d" % (w, h, ws / 2 - w / 2, hs / 2 - h / 2))
    UI(root, ).pack(fill="both", expand=True)
    root.mainloop()
