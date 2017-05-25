# -*- coding: utf-8 -*-


from PyQt4 import QtCore, QtGui
import glob
import subprocess
import os.path


def scan():
    return glob.glob('/dev/ttyU*') +  glob.glob('/dev/ttyA*')
    
    
try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_Programacao(object):
    def setupUi(self, Programacao):
        Programacao.setObjectName(_fromUtf8("Programacao"))
        Programacao.resize(700, 400)
        Programacao.setWindowTitle(QtGui.QApplication.translate("Programacao", "Form", None, QtGui.QApplication.UnicodeUTF8))
        
       
       # arquivo = open("/home/rjom/PSIU-controle/Teste/Teste.pde", "r")
        self.codigo = QtGui.QPlainTextEdit(Programacao)
        self.codigo.setGeometry(QtCore.QRect(20, 50, 635, 225))
       # self.codigo.setPlainText(arquivo.read()) 
       # arquivo.close()
       
        self.portaSerial = QtGui.QComboBox(Programacao)
        self.portaSerial.setGeometry(QtCore.QRect(0,0,136,28))
        self.portaSerial.addItem("Serial Port") 
        for numero in range(len(scan())):
        	self.portaSerial.addItem(scan()[numero])
        
        
        self.board = QtGui.QComboBox(Programacao)
        self.board.addItem("Board")
        self.board.setGeometry(QtCore.QRect(140,0,136,28))
        self.board.addItem("Arduino Duemilanove W/ aTMega 328")
        self.board.addItem("Arduino Mega 2056")
        self.board.addItem("Arduino Uno")

        
	self.upload = QtGui.QPushButton(Programacao)
        self.upload.setGeometry(QtCore.QRect(550, 5, 100, 40))
        self.upload.setText(QtGui.QApplication.translate("Upload", "Upload", None, QtGui.QApplication.UnicodeUTF8))
        self.upload.setObjectName(_fromUtf8("upload"))
        
        

        
        self.retranslateUi(Programacao)
        QtCore.QMetaObject.connectSlotsByName(Programacao)

    
    def retranslateUi(self, Programacao):
        pass
    
    
    def escolhaPortaSerial(self, text):
    	print 'oi!'
    
    def escolhaBoard(self, text): 
        if(text ==  "Arduino Uno"):
        	print "uno!"
        elif(text == "Arduino Duemilanove W/ aTMega 328"):
        	print "duemilanove!"
        	
        elif(text == "Arduino Mega 2056"):
        	print "MegaZORD!!! HAHA"
    
    def uploadFunction(self):
    	stringCaminho = os.path.expanduser("~/") + "PSIU-Controle/Teste/Teste.pde"
        novoarquivo = open(stringCaminho, "w")
        string = str(self.codigo.toPlainText())
        novoarquivo.write(string)
        novoarquivo.close()
   	subprocess.call(['./upload.sh'])
    
if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    Programacao = QtGui.QWidget()
    ui = Ui_Programacao()
    ui.setupUi(Programacao)
    Programacao.show()
    sys.exit(app.exec_())
