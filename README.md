# Easen bit-level file operations

This tools were intended to help debugging SDR communications easier on my BSc thesis. Here's the list:

| Utility | Description | Language | Location
|--|--|--|--|
|**fconv**|Convert between data between text, binary and hexadecimal formats.| bash|scripts/
|**binarize**|Converts data to stream of bits in a text file | bash | scripts/
|**patfinder**| Designed to synchronize symbols from raw demodulated data on wireless communications | C | patfinder/

## fconv

    $ fconv
    
    Usage: fconv -i input_file [ -f from_format ] -t to_format [ -o output file ]
    Swiss army knife for ascii - hex - bin conversion of text files
      -i  File to convert
      -f  Format of file to convert. Automatically determined when not specified
      -o  Write output to this file. Stdout when not specified
      -t  Output format
      -c  Number of output chars per line
      -h  Print this help

#### Example of usage

Create a file

    $ echo "6661 7066 6170 6661 700a" > file

Convert it to a text file using fconv 

    $ fconv -i file -t ascii
    fapfapfap

Alternatively, write result to a file

    $ fconv -i file -t ascii -o textfile

Convert to a binary string (two ways)

    $ fconv -i textfile -t bin -o binfile
    $ cat binfile
    01100110011000010111000001100110011000010111000001100110011000010111000000001010
    $ fconv -i file -t bin
    01100110011000010111000001100110011000010111000001100110011000010111000000001010

## binarize

This actually is an early fconv version, but I kept it for simplicity

    $ binarize

    Unpack bytes in file to bits to easen manual pattern matching
    Usage: binarize input_file [ output_file ]

#### Example of usage

Create a file

    $ echo "fapfapfap" > me

Convert it to a binary string on a file (or alternatively to stdout)

    $ ./binarize me there
    >> Output written to there <<
     
Check out the resulting string
  
    $ cat there
    01100110011000010111000001100110011000010111000001100110011000010111000000001010

## patfinder

Searches for the beginning of a bit sequence on a file. When matched, dumps the contents of the file from the beginning of the pattern on.

Note this tool does not try to match the whole pattern, but just the first N\*8 bits.

    $ patfinder
    
    Usage: patfinder [ N ] [ pattern_file ] file_to_match
      N: number of bytes to match at the beginning
      pattern_file: original from which to obtain the pattern
      file_to_match: received file, where the pattern must be found

#### Building

    $ cd patfinder/
    $ make

#### Example of usage

Sample files: one containing pattern, the other 

    $ echo "sample text" > sample
    $ echo "dshkjsgdhkl sam kgds hgd sample textsdjf"> longsequence
  
Simulate symbol offset

    $ fconv -i longsequence -t bin -o longsequence.bin
    $ sed -i 's/^/111/' longsequence.bin
    $ fconv -i longsequence.bin -t ascii -o longsequence.dat

Search first 4 bytes of the pattern
    
    $ patfinder 4 sample longsequence.dat
     
    File: sample. Pattern to match:
        'sampl'
        0x73 0x61 0x6d 0x70 0x6c
    Searching longsequence.dat
    Trying char 29... success (bit offset = 3)
    Text written to /tmp/patfound:
        sample textsdjf�

Note the output does not contain exclusively the sample, but also the rest of the file.
