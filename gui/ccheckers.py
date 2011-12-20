#!/usr/bin/env python

import sys
import cc
from PyQt4 import QtCore, QtGui

grid = [[9,9,9,9,9,9,9,9,9,9,9,9,1,9,9,9,9,9,9,9,9,9,9,9,9],
        [9,9,9,9,9,9,9,9,9,9,9,1,9,1,9,9,9,9,9,9,9,9,9,9,9],
        [9,9,9,9,9,9,9,9,9,9,1,9,1,9,1,9,9,9,9,9,9,9,9,9,9],
        [9,9,9,9,9,9,9,9,9,1,9,1,9,1,9,1,9,9,9,9,9,9,9,9,9],
        [4,9,4,9,4,9,4,9,0,9,0,9,0,9,0,9,0,9,6,9,6,9,6,9,6],
        [9,4,9,4,9,4,9,0,9,0,9,0,9,0,9,0,9,0,9,6,9,6,9,6,9],
        [9,9,4,9,4,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,6,9,6,9,9],
        [9,9,9,4,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,6,9,9,9],
        [9,9,9,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,9,9,9],
        [9,9,9,5,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,3,9,9,9],
        [9,9,5,9,5,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,3,9,3,9,9],
        [9,5,9,5,9,5,9,0,9,0,9,0,9,0,9,0,9,0,9,3,9,3,9,3,9],
        [5,9,5,9,5,9,5,9,0,9,0,9,0,9,0,9,0,9,3,9,3,9,3,9,3],
        [9,9,9,9,9,9,9,9,9,2,9,2,9,2,9,2,9,9,9,9,9,9,9,9,9],
        [9,9,9,9,9,9,9,9,9,9,2,9,2,9,2,9,9,9,9,9,9,9,9,9,9],
        [9,9,9,9,9,9,9,9,9,9,9,2,9,2,9,9,9,9,9,9,9,9,9,9,9],
        [9,9,9,9,9,9,9,9,9,9,9,9,2,9,9,9,9,9,9,9,9,9,9,9,9]]

class Point:
    x = 0
    y = 0

class Ccheckers(QtGui.QMainWindow):
    def __init__(self):
        QtGui.QMainWindow.__init__(self)
        self.setGeometry(300, 300, 500, 500)
        self.setWindowTitle('Chinese Man Checkers')
	self.ccboard = Board(self)
	self.statusbar = self.statusBar()

        fileMenu = self.menuBar().addMenu("&File")
        buttonBox = QtGui.QDialogButtonBox(QtGui.QDialogButtonBox.Ok)
        layout = QtGui.QVBoxLayout()
        layout.addWidget(self.ccboard)
        layout.addWidget(buttonBox)
        self.setLayout(layout)

        self.setCentralWidget(self.ccboard)


	self.connect(self.ccboard, QtCore.SIGNAL("messageToStatusbar(QString)"),
                     self.statusbar, QtCore.SLOT("showMessage(QString)"))
	self.ccboard.start()
        self.center()

    def center(self):
        screen = QtGui.QDesktopWidget().screenGeometry()
        size = self.geometry()
        self.move((screen.width()-size.width())/2,
                  (screen.height()-size.height())/2)
#-end-of Ccheckers


class Board(QtGui.QFrame):
    BoardWidth = 10
    BoardHeight = 22
    Speed = 300
    click = Point()
    selected = Point()

    def __init__(self, parent):
        QtGui.QFrame.__init__(self, parent)
        self.timer = QtCore.QBasicTimer()
        self.isWaitingAfterLine = False
        self.curX = 0
        self.curY = 0
        self.numLinesRemoved = 0
        self.board = []
        self.setFocusPolicy(QtCore.Qt.StrongFocus)
        self.isStarted = False
        self.isPaused = False
        self.isselected = False

    def start(self):
        if self.isPaused:
            return
        self.isStarted = True
        self.isWaitingAfterLine = False
        self.emit(QtCore.SIGNAL("messageToStatusbar(QString)"), "start")
        self.timer.start(Board.Speed, self)

    def paintEvent(self, event):
        # Construction in the paintEvent OK PyQt4/html/qpainter.html#details
        painter = QtGui.QPainter(self)
        painter.setRenderHints(QtGui.QPainter.Antialiasing)
        for i in range(len(grid)):
            for j in range(len(grid[0])):
                v = g.get_board_val(i,j)
                if v != cc.board.INVALID_SITE:
                    drawMarble(painter, j*case, 1.35*i*case, v+1)

        i = int(self.click.y / (1.35*case)) # lines
        j = int(self.click.x / case)        # columns

        # store selected.i
        desti = int(self.selected.y / (1.35*case))
        destj = int(self.selected.x / case)

        #v = grid[i][j]
        v = g.get_board_val(i,j)
        #if marble selected and destination free
        if self.isselected and v == -1:
            self.isselected = False
            element = QtGui.QPainterPath()
            element.addEllipse(j*case-2, i*(1.35*case)-2, case+4, case+4)
            painter.setPen(QtGui.QColor(0xFF0000))
            painter.drawPath(element)
            if (cc.play(g, "move "+str(desti)+" "+str(destj)+" "+str(i)+" "+str(j)+" 1")):
                swap(desti, destj, i, j)
            g.next_round() #FIXME: Add button "next round"
            QtGui.QWidget.update(self)
        #else marble selected and dest is the same as src
        elif self.isselected and desti == i and destj == j:
            self.isselected = False
        #else dest owns a marble
        elif v != cc.board.INVALID_SITE and v != -1:
            print v
            self.isselected = True
            self.selected.x = self.click.x
            self.selected.y = self.click.y
            element = QtGui.QPainterPath()
            element.addEllipse(j*case-2, i*(1.35*case)-2, case+4, case+4)
            painter.setPen(QtGui.QColor(0x000000))
            painter.drawPath(element)
        #anything else
        else:
            self.isselected = False

    def timerEvent(self, event):
        QtGui.QFrame.timerEvent(self, event)

    def mouseReleaseEvent(self, event):
        if not self.isStarted:
            QtGui.QWidget.mouseReleaseEvent(self, event)
            return
        x = event.x()
        y = event.y()
        if x != self.click.x and y != self.click.y:
            self.click.x = x
            self.click.y = y
            QtGui.QWidget.update(self)
            self.emit(QtCore.SIGNAL("messageToStatusbar(QString)"),
                      str(x) + " " + str(y))
#-end-of Board


def drawMarble(painter, x, y, shape):
    colorTable = [0x000000, 0xCC6666, 0x66CC66, 0x6666CC,
                  0xCCCC66, 0xCC66CC, 0x66CCCC, 0xDAAA00]
    color = QtGui.QColor(colorTable[shape])
    element = QtGui.QPainterPath()
    element.addEllipse(x + 1, y + 1, case-2, case-2)
    painter.fillPath(element, color)
    painter.setPen(color.dark())
    painter.drawPath(element)

def swap(i1, j1, i2, j2):
    tmp = grid[i1][j1]
    grid[i1][j1] = grid[i2][j2]
    grid[i2][j2] = tmp

#C++ machinery
bb = cc.board()
g  = cc.game(bb,2)

#Graphical interface
case = 20;
app = QtGui.QApplication(sys.argv)
ccheckers = Ccheckers()
ccheckers.show()
sys.exit(app.exec_())

