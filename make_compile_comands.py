
CXX = "g++"
CXX_FLAGS = "-std=c++20 -Wall -Wextra -Iinclude"

FILES = [
    "errlog",
    "main",
    "scanner",
    "token",
    "ast_printer"
]

if __name__ == "__main__":
    commands = [
        {
            "directory": ".",
            "command": f"{CXX} {CXX_FLAGS} -c src/{file}.cpp -o build/{file}.o",
            "file": f"src/{file}.cpp"
        }
        for file in FILES
    ]

    import json
    with open("./compile_commands.json", "w") as f:
        print(json.dumps(commands, indent = 4))
        json.dump(commands, f, indent=4)
    
    # bros hacking my project
    # you will be paid $9.99 an hour until this project is done.
    # lol :X
    # writin:wg the compitle commands is too much effort,
    # esp to change it with a new file so.... just write a script lol XD
    # anyways, this is approx ready i think

    # okie dokie, TY
    # food time.

    # oki, i done :)

