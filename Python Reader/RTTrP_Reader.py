import socket
import binascii
import struct
import thirdParty_motion
import thirdParty_lighting
import traceback
import RTTrP

def openConnection(IP, PORT, isReading):
	"""
		IP:			The IP to listen on for RTTrPM packets
		Port:		The Port to listen on for RTTrPM packets from the given IP
		isReading:	A signal for the thread running this function, when triggered, the connection will close
		OutModules:	TBD, will be used to update fields in the GUI; Removed for now

		Description:
					
					This function is where the packets are read in and split into their
					various modules. It will determine the type of packet and begin
					disecting it into its various modules (depending on what is present).
	"""

	UDP_IP = str(IP)
	UDP_PORT = int(PORT)

	# Create the socketm then bind it to the (IP, PORT) pair
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock.bind((UDP_IP, UDP_PORT))

	count = 1
	data = 1 
	addr = 1

	# So long as the signal is set, then we will continue to listen on the given (IP, PORT) socket
	while isReading.isSet():
		# Set the size of the buffer to the maximum UDP packet size, and retrieve the data sent
		data, addr = sock.recvfrom(65535)

		if(data != None and addr != None):
			
			# Create the RTTrP header
			pkt = RTTrP.RTTrP(data)

			try:
				# If we have an RTTrPM packet, begin to extract the components
				if (hex(pkt.fltHeader) == "0x4334" or hex(pkt.fltHeader) == "0x3443"):
					pkt = thirdParty_motion.RTTrPM(pkt)

					# After determining the number of trackables (listed in the RTTrP header) we begin to extract
					# each trackable from the packet and return it to the GUI
					for i in range(pkt.rttrp_head.numMods):
						module = thirdParty_motion.Trackable(pkt.data, pkt.rttrp_head.intHeader, pkt.rttrp_head.fltHeader)

						pkt.trackable = module
	
						# For each trackable, we need to extract each module. Keep in mind when dealing with LED modules, each
						# individual LED is considered it's own separate module, so we don't need to worry about
						# modules within modules, except in the case of a Trackable.
						for i in range(module.numMods):
							if (module.data[0] == 2):
								pkt.centroidMod = thirdParty_motion.CentroidMod(module.data, module.intSig, module.fltSig)
								module.data = pkt.centroidMod.data
							elif (module.data[0] == 3):
								pkt.quatMod = thirdParty_motion.QuatModule(module.data, module.intSig, module.fltSig)
								module.data = pkt.quatMod.data
							elif (module.data[0] == 4):
								pkt.eulerMod = thirdParty_motion.EulerModule(module.data, module.intSig, module.fltSig)
								module.data = pkt.eulerMod.data
							elif (module.data[0] == 6):
								pkt.ledMod.append(thirdParty_motion.LEDModule(module.data, module.intSig, module.fltSig))
								module.data = pkt.ledMod[len(pkt.ledMod)-1].data
							elif (module.data[0] == 32):
								pkt.centroidAccVelMod = thirdParty_motion.CentroidAccVelMod(module.data, module.intSig, module.fltSig)
								module.data = pkt.centroidAccVelMod.data
							elif (module.data[0] == 33):
								pkt.LEDAccVelMod.append(thirdParty_motion.LEDAccVelMod(module.data, module.intSig, module.fltSig))
								module.data = pkt.LEDAccVelMod[len(pkt.LEDAccVelMod)-1].data
							elif (module.data[0] == 34):
								pkt.ZoneColMod = thirdParty_motion.ZoneColMod(module.data, module.intSig, module.fltSig)
								module.data = pkt.ZoneColMod.data
							else:
								# unknown packet type, da fuq is this
								exit()
				elif (hex(pkt.fltHeader) == "0x4434" or hex(pkt.fltHeader) == "0x3444"): # TODO: Create the RTTrPL code that reads an RTTrPL packet
					pkt = RTTrPL(pkt)
					sock.close()
					exit()
			except Exception as e:
				print(traceback.print_exc(None))
				continue

			pkt.printPacket()
			print("===========================================")

	sock.close()
