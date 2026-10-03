import sys
from pathlib import Path

target_letters = {'а', 'е', 'о', 'р', 'с', 'у', 'х', 'А', 'В', 'Е', 'К', 'О', 'Р', 'С', 'Т', 'Х'}

substitution_map = {
    'а': 'a', 'е': 'e', 'о': 'o', 'р': 'p', 'с': 'c', 'у': 'y', 'х': 'x',
    'А': 'A', 'В': 'B', 'Е': 'E', 'К': 'K', 'О': 'O', 'Р': 'P', 'С': 'C', 'Т': 'T', 'Х': 'X',
}

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
    # print(f"Binary representation of the message: {bitMessage}")

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

# python3 encode.py /home/nomen/Documents/sth.txt /home/nomen/Documents/secret.txt
def main(argv: list[str]) -> int:
    arguments_required = "1st - path to the container file;\n2nd - path to the message file"
    
    argc = len(argv)
    if argc != 3:
        print(f"WRONG NUMBER OF ARGUMENTS ({argc - 1})!\n{arguments_required}", file=sys.stderr)
        return 1

    file_path = Path(argv[1])
    if not file_path.exists():
        print(f"NO SUCH CONTAINER FILE: {file_path}!", file=sys.stderr)
        return 1
        
    message_path = Path(argv[2])
    if not message_path.exists():
        print(f"NO SUCH MESSAGE FILE: {message_path}!", file=sys.stderr)
        return 1

    with open(message_path, 'r', encoding='cp1251') as f:
        message = f.read()

    encode_message(file_path, message)
    return 0

if __name__ == "__main__":
    sys.exit(main(sys.argv))
