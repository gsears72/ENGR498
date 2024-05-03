import csv

def csv_to_cpp_array(csv_filename, cpp_header_filename):
    # Read CSV data
    data = []
    with open(csv_filename, newline='') as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            # Convert the necessary columns to float and store all as strings for C++ compatibility
            try:
                if len(row) >= 4:  # Ensuring there are at least 4 columns
                    converted_row = [
                        str(float(row[0])),  # C_alpha
                        str(float(row[1])),  # C_beta
                        str(float(row[2])),  # V
                        str(float(row[3]))   # Angle
                    ]
                    data.append(converted_row)
            except ValueError:
                continue  # Skip rows that cannot be converted to float

    # Write to C++ header file
    with open(cpp_header_filename, 'w') as cppfile:
        cppfile.write('#ifndef CSV_DATA_H\n')
        cppfile.write('#define CSV_DATA_H\n\n')
        
        # Declare the array
        cppfile.write('constexpr double csvData[][4] = {\n')

        for row in data:
            cppfile.write('    {')
            cppfile.write(', '.join(row))
            cppfile.write('},\n')
        
        cppfile.write('};\n\n')
        cppfile.write('#endif // CSV_DATA_H\n')
        

csv_to_cpp_array('../lookupTable.csv', '../include/CsvData.h')
