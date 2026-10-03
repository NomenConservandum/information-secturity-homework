import sys
from pathlib import Path

target_letters = {'а', 'е', 'о', 'р', 'с', 'у', 'х', 'А', 'В', 'Е', 'К', 'О', 'Р', 'С', 'Т', 'Х'}

substitution_map = {
    'а': 'a',
    'е': 'e',
    'о': 'o',
    'р': 'p',
    'с': 'c',
    'у': 'y',
    'х': 'x',
    'А': 'A',
    'В': 'B',
    'Е': 'E',
    'К': 'K',
    'О': 'O',
    'Р': 'P',
    'С': 'C',
    'Т': 'T',
    'Х': 'X',
}

def is_substitutable(char: str) -> bool:
    return char in target_letters or char in substitution_map.values()

def letter_substitution(char: str) -> str:
    if char in substitution_map:
        return substitution_map[char]
    else: # non-target letter, it can be an already substituted one
        return '-'

def get_indices_of_letters(text: str) -> list[int]:
    indices = []
    for idx, char in enumerate(text):
        if char in target_letters:
            indices.append(idx)
    return indices

def encode_message(path: Path, message: str) -> int:
    init_file_content = ""
    with open(path, 'r', encoding='cp1251') as f:
        for line in f:
            init_file_content += line
    
    bitMessage = bin(int.from_bytes(message.encode(encoding='cp1251'), 'big'))[2:]
    print(f"Binary representation of the message: {bitMessage}")

    target_letter_indices = get_indices_of_letters(init_file_content)
    target_letter_index_index = 0
    
    content_list = list(init_file_content)

    while (bitMessage):
        bit = bitMessage[len(bitMessage) - 1]
        bitMessage = bitMessage[:len(bitMessage) - 1]

        if (bit == '1'):
            # get the target letter
            target_letter = content_list[target_letter_indices[target_letter_index_index]]
            # change the target letter to its encoded form
            content_list[target_letter_indices[target_letter_index_index]] = letter_substitution(target_letter)
        target_letter_index_index += 1

    init_file_content = "".join(content_list)

    with open(path, 'w', encoding='cp1251') as f:
        f.write(init_file_content)
    
    return 0


def decode_message(path: Path) -> int:
    init_file_content = ""
    with open(path, 'r', encoding='cp1251') as f:
        for line in f:
            init_file_content += line

    bitMessage = ""
    for l in init_file_content:
        if (is_substitutable(l)):
            if letter_substitution(l) == '-': # non Russian letter - it was substituted
                bitMessage = '1' + bitMessage
            else: # original Russian letter
                bitMessage = '0' + bitMessage
    
    if len(bitMessage) % 8 != 0: # adding zero-bits for the last byte
        bitMessage = '0' * (8 - len(bitMessage) % 8) + bitMessage

    print(f"Binary representation of the message: {bitMessage}")

    decoded_message = bytes(int(bitMessage[i:i+8], 2) for i in range(0, len(bitMessage), 8)).decode(encoding='cp1251')
    decoded_message = decoded_message.lstrip('\x00') # removing null characters
    print(f"Decoded message: {decoded_message}")
    
    return 0

# E.g. python3 task2.py /home/nomen/Documents/sth.txt 2
def main(argv: list[str]) -> int:
    arguments_required = "1st - path to the file;\n2nd - mode (1 for encode and 2 for decode)\n3rd - message to encode (if mode 1 is chosen)"
    

    # argv[0] is the program name, so we look at argv[1:]
    argc = len(argv)

    if (
        argc < 3
        or argc > 4
        or (argv[2] == '1' and argc != 4)
        or (argv[2] == '2' and argc != 3)
    ):
        print(f"WRONG NUMBER OF ARGUMENTS ({argc - 1})!\n{arguments_required}",
              file=sys.stderr)
        return 1

    file_path = Path(argv[1])

    if not file_path.exists():
        print(f"NO SUCH FILE: {file_path}!", file=sys.stderr)
        return 1

    # Branching on mode
    if argv[2] == '1':
        encode_message(file_path, argv[3])
    elif argv[2] == '2':
        decode_message(file_path)
    else:
        print(f"WRONG MODE! The arguments needed:\n{arguments_required}",
              file=sys.stderr)
        return 1

    return 0

main(sys.argv)