#!/usr/bin/python3

from tkinter import *
from time import gmtime, strftime, sleep

class MyClock:
	def __init__(self):
		self.root = Tk()
		self.root.title("clock")
		self.root.resizable(width=False, height=False)
		self.root.config(bg="red")
		self.label = Label(self.root, font=('', 20, 'bold'), bg='green')
		self.label.pack()

	def Hour(self):
		self.label.config(text=strftime('%H:%M:%S'))
		self.label.after(200, self.Hour)

	def run(self):
		self.Hour()

	def mainloop(self):
		self.root.mainloop()

if __name__ == "__main__":
	clock = MyClock()
	clock.run()
	clock.mainloop()

