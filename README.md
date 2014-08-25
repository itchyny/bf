# bf - a unix command which executes a Brainfuck code
## Installation

    git clone https://github.com/itchyny/bf
    cd ./bf
    autoreconf -i
    ./configure
    make
    sudo make install

## Example

    echo "++++++++[>+++++++++>+++++++++++++>++++++>++++++<<<<-]>.>---.+++++++..+++.>----.------------.<++++++++.--------.+++.------.--------.>>--." | bf
    Hello, world.

## Author
itchyny <https://github.com/itchyny>

## Repository
https://github.com/itchyny/bf

## License
This software is released under the MIT License, see LICENSE.
