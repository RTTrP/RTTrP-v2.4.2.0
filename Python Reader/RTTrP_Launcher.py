import _thread
import sys
import RTTrP_Reader
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
import threading
import subprocess

class MainWindow(QMainWindow):
	# Creates the main window for the GUI
	def __init__(self, parent=None):
		super(MainWindow, self).__init__(parent)

		self.centralWidget = QTabWidget()

		# Add the tab for RTTrPM and RTTrPL
		self.centralWidget.addTab(RTTrPMTab(self), "RTTrPM")
		self.centralWidget.addTab(RTTrPLTab(self), "RTTrPL")

		self.setCentralWidget(self.centralWidget)

		# Set window size
		self.setFixedSize(640, 480)

		self.setWindowTitle("RTTrP Listener")
	
class RTTrPMTab(QWidget):
	def __init__(self, parent):
		# Initialize the RTTrPM tab
		super(RTTrPMTab, self).__init__(parent)

		IP = QLabel("IP:")
		PORT = QLabel("Port:")

		self.submitButton = QCheckBox("Start Listening")
		self.submitButton.setChecked(False)
		self.IP = QLineEdit()
		self.PORT = QLineEdit()
		
		buttonLayout1 = QVBoxLayout()
		buttonLayout1.addWidget(self.submitButton)
		buttonLayout1.addWidget(IP)
		buttonLayout1.addWidget(self.IP)
		buttonLayout1.addWidget(PORT)
		buttonLayout1.addWidget(self.PORT)

		mainLayout = QGridLayout()
		mainLayout.addLayout(buttonLayout1, 0, 1)
		
		self.setLayout(mainLayout)
	
		self.submitButton.clicked.connect(self.startListening)

	def startListening(self):
		IP = self.IP.text()
		PORT = self.PORT.text()

		if (self.submitButton.isChecked()):
			if (IP == "") or (PORT == ""):
				# Print a generic message to inform the user that we cannot listen on "nothing"
				QMessageBox.information(self, "Empty Field", "Please enter a valid IP and PORT.")
				self.submitButton.setChecked(False)
				return
			else:
				# If IP and PORT are good, then change the text and launch a thread that starts listening to the given IP 
				# and PORT for RTTrPM packets
				self.submitButton.setText("Stop Listening")
				self.beginReading = threading.Event()

				# Set the signal to True so that the thread will execute the code in openConnection
				self.beginReading.set()
	
				self.reader = threading.Thread(None, RTTrP_Reader.openConnection, None, (IP, PORT, self.beginReading))
				self.reader.start()
		elif (not self.submitButton.isChecked()):
			# When the box is unchecked, we need to alert the worker thread to stop
			self.submitButton.setText("Start Listening")

			# Clear the signal, which causes the worker thread to close the connection
			self.beginReading.clear()

class RTTrPLTab(QWidget):
	def __init__(self, parent):
		# Initialize the RTTrPL tab
		super(RTTrPLTab, self).__init__(parent)

		IP = QLabel("IP:")
		PORT = QLabel("Port:")

		self.submitButton = QCheckBox("Start Listening")
		self.submitButton.setChecked(False)
		self.IP = QLineEdit()
		self.PORT = QLineEdit()
		
		buttonLayout1 = QVBoxLayout()
		buttonLayout1.addWidget(self.submitButton)
		buttonLayout1.addWidget(IP)
		buttonLayout1.addWidget(self.IP)
		buttonLayout1.addWidget(PORT)
		buttonLayout1.addWidget(self.PORT)

		mainLayout = QGridLayout()
		mainLayout.addLayout(buttonLayout1, 0, 1)
		
		self.setLayout(mainLayout)
	
		self.submitButton.clicked.connect(self.startListening)

	def startListening(self):
		IP = self.IP.text()
		PORT = self.PORT.text()

		if (self.submitButton.isChecked()):
			if (IP == "") or (PORT == ""):
				# Print a generic message to inform the user that we cannot listen on "nothing"
				QMessageBox.information(self, "Empty Field", "Please enter a valid IP and PORT.")
				self.submitButton.setChecked(False)
				return
			else:
				# If IP and PORT are good, then change the text and launch a thread that starts listening to the given IP 
				# and PORT for RTTrPL packets
				self.submitButton.setText("Stop Listening")
				self.beginReading = threading.Event()
	
				# Set the signal to True so that the thread will execute the code in openConnection
				self.beginReading.set()
				
				#self.reader = threading.Thread(None, RTTrP_Reader.openConnection, None, (IP, PORT, self.beginReading, self.modules))
				#self.reader.start()
		elif (not self.submitButton.isChecked()):
			# When the box is unchecked, we need to alert the worker thread to stop
			self.submitButton.setText("Start Listening")

			# Clear the signal, which causes the worker thread to close the connection
			self.beginReading.clear()

if __name__ == '__main__': # Technically this isn't necessary, but we can leave it here.

	app = QApplication(sys.argv)
	screen = MainWindow()
	screen.show()

	sys.exit(app.exec_())
