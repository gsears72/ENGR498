ENGR498 Zephyrus Code README

How to use:
1. I highly recommend using platformIO to build this project. That is the tool I used.
2. Open a powershell or command prompt window.
3. Install the necessary python packages as outlined in the 'installed_packages.txt' file.
4. Run the 'generate_characterization_header.py' script.
	4.a. This loads the data from lookupTable.csv into a 2D double array and stores it in the 'CsvData.h' file.
5. Load the C code onto the board with the 'upload' command in platformIO.
6. Run the 'TestingScript.py' script to interact with the electronics.


Notes:
1. All C++ source code is located in the 'src' folder. All associated header files are in the 'include' folder.

2. The components of the code have been broken out to be as modular as needed.
	2.a. All communications with the sensors are located within the 'sensor_communications folder'. The 'sensor_communications.cpp'
			file contains all of the higher level functions to poll a single sensor's data or pressure, as well as all of the sensors at once.
	2.b. All data is stored in the 'CoriolisProbe' struct currently. This gives you the option of printing the data for debugging
			as well as giving you a starting point for adding functionality.

3. Currently, there us nothing stopping any function with the correct header file included from using the functions in the file. Converting
	a lot of the functions to be C++ class member functions would be a safer way to store the information. However, the size of the C code
	as it stands now is 15% of the system Flash memory. Just storing the vector information and calculations takes up another 60%. A better
	optimization may just be to convert the vector into C style to save space, allowing more room to store a higher definition lookup table.

4. The serial port that the board is set to communicate over is defined in the platformio.ini file.

5. I didn't program the vectormath.cpp programs at all. I attempted to make them integrate with my code but there is a bug somewhere in there
	that is making the vectors not calculate correctly. You can see the output of the calculations by running the 'TestingScript.py'