
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
    

