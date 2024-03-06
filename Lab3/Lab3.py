import struct

# Function to hide a byte into a pixel
def hide_byte_into_pixel(pixel, hide_byte):
    if hide_byte is not None:
        pixel[0] &= 0xFC
        pixel[0] |= (hide_byte >> 6) & 0x3
        pixel[1] &= 0xFC
        pixel[1] |= (hide_byte >> 4) & 0x3
        pixel[2] &= 0xFC
        pixel[2] |= (hide_byte >> 2) & 0x3
        pixel[3] &= 0xFC
        pixel[3] |= hide_byte & 0x3

# Function to extract a byte from a pixel
def extract_byte_from_pixel(pixel):
    return ((pixel[0] & 0x3) << 6) | ((pixel[1] & 0x3) << 4) | ((pixel[2] & 0x3) << 2) | (pixel[3] & 0x3)

# Function to hide data in a BMP file
def hide_data_in_bmp(input_bmp, output_bmp, data):
    with open(input_bmp, 'rb') as f:
        # Read BMP file header
        header = f.read(54)
        output_data = bytearray(header)

        # Hide data in pixels
        byte_index = 0
        while True:
            pixel = bytearray(f.read(4))
            if not pixel:
                break
            if byte_index < len(data):
                hide_byte_into_pixel(pixel, data[byte_index])
                output_data.extend(pixel)
                byte_index += 1
            else:
                # If we've reached the end of the data, append remaining pixels unchanged
                output_data.extend(pixel)

    with open(output_bmp, 'wb') as f:
        f.write(output_data)

# Function to extract data from a BMP file
def extract_data_from_bmp(bmp_file):
    with open(bmp_file, 'rb') as f:
        # Skip BMP file header
        f.seek(54)
        data = []
        while True:
            pixel = bytearray(f.read(4))
            extracted_byte = extract_byte_from_pixel(pixel)
            if extracted_byte == 0xFF:  # End of file marker
                break
            data.append(extracted_byte)
    return bytes(data)

# Example usage:
input_bmp = "/home/rad/VisualStudioCode_reps/UnversityEducation_ProtectInformation/Lab3/5.bmp"
output_bmp = "/home/rad/VisualStudioCode_reps/UnversityEducation_ProtectInformation/Lab3/output.bmp"
data_to_hide = b""
hide_data_in_bmp(input_bmp, output_bmp, data_to_hide)

extracted_data = extract_data_from_bmp(output_bmp)

# Try decoding with different encodings
for encoding in ['utf-8', 'cp1251', 'koi8-r']:
    try:
        decoded_data = extracted_data.decode(encoding)
        print("Extracted data ({}): {}".format(encoding, decoded_data))
        break
    except UnicodeDecodeError:
        continue
else:
    print("Unable to decode the extracted data with known encodings.")

