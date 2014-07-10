## Speaker Notes:

### Alternate Data Streams Demo
- Open command prompt and navigate to \demos\ads
- Show PDF of PCAST report.
- issue dir command to highlight file sizes
  - note the file size, dates, etc.
- notepad pcast-nitrd-report-2010.pdf:secret.txt
  - type secret message, save, close
- Show the file details of the pcast report
  - should be identical to previous
- show/read the hidden file 
  - notepad pcast-nitrd-report-2010.pdf:secret.txt

### Image Stego
- Overview:
  - open the Visual Studio solution \demos\lsb\lsb.sln
  - make sure VS is in Preso mode (PresentOn)
  - describe the two apps (HideMessage.exe, ReadMessage.exe)
  - discuss HideMessage method (header, etc.)
  - talk about WriteDataToImage method
  - talk about little endian vs. big endian
  - walk through code in "ReadMessage" program.cs
  - _briefly_ discuss casting
- show them
  - open command prompt to \demos\lsb
  - type something similar to the following:
    - HideMessage\bin\x64\Debug\HideMessage.exe -i vacation-house.png -m "Welcome to my talk on LSB" -o v01.png
  - open and show them both images
  - ReadMessage\bin\x64\Debug\ReadMessage.exe -i v01.png

### Network Stego

