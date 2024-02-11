import sys

def hide_byte_into_pixel(pixel, hide_byte):
    pixel[0] &= 0xFC
    pixel[0] |= (hide_byte >> 6) & 0x3
    pixel[1] &= 0xFC
    pixel[1] |= (hide_byte >> 4) & 0x3
    pixel[2] &= 0xFC
    pixel[2] |= (hide_byte >> 2) & 0x3
    pixel[3] &= 0xFC
    pixel[3] |= hide_byte & 0x3

def extract_byte_from_pixel(pixel):
    return ((pixel[0] & 0x3) << 6) | ((pixel[1] & 0x3) << 4) | ((pixel[2] & 0x3) << 2) | (pixel[3] & 0x3)

def hide_data_in_image(input_image, output_image, data_file):
    with open(input_image, 'rb') as img_file:
        img_data = bytearray(img_file.read())

    with open(data_file, 'rb') as data:
        data_to_hide = data.read()
    
    data_to_hide += bytes([0xFF])  # End of file marker
    
    if len(data_to_hide) * 4 > len(img_data) - 54:
        print("Error: Input image is too small to hide data")
        return

    idx = 54  # Start of pixel data
    for byte in data_to_hide:
        hide_byte_into_pixel(img_data[idx:idx+4], byte)
        idx += 4

    with open(output_image, 'wb') as out_img:
        out_img.write(img_data)

    print("Data hidden successfully.")

def extract_data_from_image(image):
    with open(image, 'rb') as img_file:
        img_data = img_file.read()

    extracted_data = bytearray()
    idx = 54  # Start of pixel data

    while True:
        byte = extract_byte_from_pixel(img_data[idx:idx+4])
        idx += 4
        if byte == 0xFF:
            break
        extracted_data.append(byte)

    return extracted_data

def main():
    if len(sys.argv) < 2:
        print("Usage: python steganography.py <mode> <input_image> <output_image> <data_file>")
        return

    mode = sys.argv[1]
    input_image = sys.argv[2]
    output_image = sys.argv[3]
    data_file = sys.argv[4]

    if mode == 'hide':
        hide_data_in_image(input_image, output_image, data_file)
    elif mode == 'extract':
        extracted_data = extract_data_from_image(input_image)
        with open(output_image, 'wb') as out_file:
            out_file.write(extracted_data)
        print("Data extracted successfully.")
    else:
        print("Invalid mode. Please use 'hide' or 'extract'.")

if __name__ == "__main__":
    main()
