import math
import os
import array
#4 parameters, 4 coding rates, 6 spreading factors
# data[SF][CR] = [BER, SNR, RSSI, Dropped Packets]
data = [ [[0] * 4 for i in range(4)] for i in range(6)]
directory = os.path.dirname(os.path.realpath('__file__'))

spreading_factor = ['7', '8', '9', '10', '11', '12']
coding_rate = ['5', '6', '7', '8']
#Test Folders
freq_offsets = ['m50k', 'm40k', 'm30k', 'm20k', 'm10k', 'noo', 'p10k', 'p20k', 'p30k', 'p40k', 'p50k']
#Compile into a single folder
                
#Aggregate test file
formatted_data = "formatted_data.csv"
file1 = os.path.join(directory, "After_Isolation/Aggregate_Data/" + formatted_data)
aggregate = open(file1, 'w')

#Index through all tests
j = 0
for cr in range(4):
    #Split into different coding rates
    for freqs in freq_offsets:
        aggregate.write(freqs + ", Coding Rate = {}\n".format(cr + 5))
        dataFile = freqs + '_'+ str(cr + 5) + ".csv"
        file2 = os.path.join(directory, "After_Isolation/Aggregate_Data/" + dataFile)
        testData = open(file2, 'w')
        testData.write("Spreading Factor, Coding Rate, BER, Dropped Packets, SNR, RSSI\n")
        aggregate.write("Spreading Factor, BER, Dropped Packets\n")

        for sf in range(6):
            BER_filename =  "data_{}_{}_{}_{}.txt".format("500kHz", coding_rate[cr], spreading_factor[sf], freqs) #input("Enter the filename to analyze:")
            file3 = os.path.join(directory, 'After_Isolation/' + freqs + "/"+ BER_filename)
            BER_data = open(file3, 'r', 1028)
            j += 1
            SNR = 0
            RSSI = 0
            errors = 0
            numBits = 0
            i = 0
            droppedPackets = 0
            while(True):
                #split into line a and line b
                readlinea = BER_data.readline().split(' ')
                readlineb = BER_data.readline().split(' ')
                if(readlinea == readlineb):
                    break
                if(len(readlineb) == 5 and len(readlinea) == 3):
                    #Create int number for transmit data, receive data
                    txNum = readlinea[0]
                    
                    errors += bin(int("0x" + readlinea[2], 16)^int("0x" + readlineb[4], 16)).count("1")
                    #another 255 bytes compared (2040 bits)
                    numBits += 2040

                    RSSI_hold = readlineb[1]
                    if(RSSI_hold[0] == '-'):
                        RSSI_num = -1* int(RSSI_hold[1::])
                    else:
                        RSSI_num = int(RSSI_hold)

                    RSSI += RSSI_num

                    snr_unit = readlineb[2]
                    if(snr_unit[0] == '-'):
                        snr_unit = snr_unit.split(".")
                        snr_num = int(snr_unit[0])
                        snr_num += int(snr_unit[1])/math.pow(10, len(snr_unit))
                        snr_num *= -1
                    else:
                        snr_num = int(snr_unit[0])
                        snr_num += int(snr_unit[2])/10 + int(snr_unit[3])/100

                    SNR += snr_num
                    i+= 1
                else:
                    droppedPackets += 1
            SNR_end = SNR/i
            RSSI_end = RSSI/i
            BER = errors/numBits
            if(BER == 0):
                BER = 1e-6
            
            data[sf][cr] = [BER, RSSI_end, SNR_end, droppedPackets]
            fileLineA =  "{},{},{},{},{},{}".format(spreading_factor[sf], coding_rate[cr], BER, droppedPackets, SNR_end, RSSI_end) #input("Enter the filename to analyze:")
            fileLineB =  "{},{},{}".format(spreading_factor[sf], BER, droppedPackets) #input("Enter the filename to analyze:")
            testData.write(fileLineA + "\n")
            aggregate.write(fileLineB + "\n")
            BER_data.close()
    '''
            print(BER_filename)
            print("Number of dropped packets: ", droppedPackets)
            print("Average SNR: {:.4f}".format(SNR/i))
            print("Average RSSI: {:.4f}".format(RSSI/i))
            print("BER: {:.8f}".format(errors/numBits))
            print("")'''
    aggregate.write("\n")
aggregate.close()