## Speaker Notes:

### Pre-Talk Setup:
- Open \demos\lsb\lsb.sln in Visual Studio
- Open \demos\NetworkHiding\NetworkHiding.sln in Visual Studio
- Make sure VS is in Presentation Mode (PresentOn)
- make sure all projects build/are built
- Open command prompt to \demos\ads
- Open command prompt to \demos\lsb
- Open command prompt to \demos\netdata
- Open \demos\netdata\BitTorrent.pcap in wireshark

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
- Feasibility Discussion
  - doesn't move across systems
  - ok for short-term hiding, but effectiveness is waning (on-going op)

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
- Feasibility discussion
  - almost impossible to visually detect
  - algorithmically, not that hard
  - need to have a reason to look

### Network Stego
- Overview:
  - show them the original file (BitTorrent.pcap) in WireShark
    - emphasize the TTL similarity
    - talk a bit about TTL, modern routing, and what a TTL of 128 or 255 actually means
    - acknowledge the "Black" lines, and hint that we will discuss this later
  - show them the code in VS
    - Walk through WritePcap.cpp
      - acknowledge hard-coded message: "Rob"... constrained demo based on fixed-length PCAP.
      - take string, convert each char to bit array, build single bit array of all bit arrays
      - loop through packets equal to length of message bit array
      - edit the MSB based on the current slot of the bit array
    - Walk through ReadPcap.cpp
      - much of the same "ceremony" seen in WritePcap.cpp
      - pcap_loop() reads the bits, then we re-assemble the message
  - Run it
    - ..\NetworkHiding\x64\Debug\WritePcap.exe BitTorrent.pcap t01.pcap
    - dir to show file existence and size
    - ..\NetworkHiding\x64\Debug\ReadPcap.exe t01.pcap
- Feasibility discussion
  - verbosity of tcp is there for a reason
  - this is fragile... what if packet sequences are out of order, re-assembly?
  - revisit black lines... these are warnings indicating that the seq/ack analysis indicated that a prior frame wasn't captured.
  - this is not uncommon
  - the abuse you perpertrate can be applied by others (to your stream)
  - discuss hard-coded 8-bit/single byte chars, forced ASCII interpretation, lack of message header/footer, etc.


