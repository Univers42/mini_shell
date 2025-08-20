#!/bin/bash

echo "🔍 Searching for 'minishell' in your home directory..."
FOUND_SHELLS=($(find -L ~ -name "minishell" 2>/dev/null | while read file; do
    if [ -x "$file" ] && file "$file" | grep -q "executable"; then
        echo "$file"
    fi
done))

if [ ${#FOUND_SHELLS[@]} -eq 0 ]; then
    echo "❌ No executable 'minishell' binary found in your home directory."
    exit 1
fi

if [ ${#FOUND_SHELLS[@]} -gt 1 ]; then
    selected=0
    while true; do
        clear
        echo "⚠️  Multiple minishell binaries found. Use arrow keys to select:"
        echo
        
        for i in "${!FOUND_SHELLS[@]}"; do
            if [ $i -eq $selected ]; then
                echo -e "\033[1;32m▶ ${FOUND_SHELLS[$i]}\033[0m"
            else
                echo "  ${FOUND_SHELLS[$i]}"
            fi
        done
        
        echo
        echo -e "\033[0;36mUse ↑/↓ arrows to navigate, Enter to select, q to quit\033[0m"
        
        read -rsn1 input
        case $input in
            $'\x1b')
                read -rsn2 input
                case $input in
                    '[A')
                        ((selected--))
                        if [ $selected -lt 0 ]; then
                            selected=$((${#FOUND_SHELLS[@]}-1))
                        fi
                        ;;
                    '[B')
                        ((selected++))
                        if [ $selected -ge ${#FOUND_SHELLS[@]} ]; then
                            selected=0
                        fi
                        ;;
                esac
                ;;
            '')
                MINISHELL_PROG="${FOUND_SHELLS[$selected]}"
                break
                ;;
            'q'|'Q')
                echo
                echo "Aborted."
                exit 1
                ;;
        esac
    done
    
    clear
    echo "✅ Selected: $MINISHELL_PROG"
else
    MINISHELL_PROG="${FOUND_SHELLS[0]}"
    echo "✅ Found minishell: $MINISHELL_PROG"
fi

read -p "Is this path correct? (y/n): " confirm
if [[ "$confirm" != "y" && "$confirm" != "Y" ]]; then
    echo "Aborting."
    exit 1
fi

if [ ! -x "$MINISHELL_PROG" ]; then
    echo "❌ Error: '$MINISHELL_PROG' is not executable."
    exit 1
fi

echo "📝 Current shell: $SHELL"

echo "🧪 Testing minishell..."
echo "exit" | "$MINISHELL_PROG"
if [ $? -ne 0 ]; then
    echo "❌ Minishell test failed. Aborting."
    exit 1
fi

MINISHELL_DIR=$(dirname "$MINISHELL_PROG")
if ! grep -q "$MINISHELL_DIR" ~/.bashrc 2>/dev/null; then
    echo "export PATH=\$PATH:$MINISHELL_DIR" >> ~/.bashrc
    echo "✅ Added $MINISHELL_DIR to PATH in ~/.bashrc"
fi

if ! grep -q "# Auto-start minishell" ~/.bashrc 2>/dev/null; then
    cat >> ~/.bashrc << EOF

# Auto-start minishell
if [[ \$- == *i* ]] && [[ -z "\$MINISHELL_ACTIVE" ]]; then
    export MINISHELL_ACTIVE=1
    echo "🚀 Starting minishell..."
    echo "💡 Type 'exit' to return to bash, Ctrl+D to close terminal"
    "$MINISHELL_PROG"
    echo "👋 Exited minishell, back to bash"
fi
EOF
    echo "✅ Added auto-start minishell to ~/.bashrc"
else
    echo "ℹ️  Auto-start minishell already configured"
fi

# Also add to .zshrc if it exists
if [[ -f ~/.zshrc ]]; then
    if ! grep -q "# Auto-start minishell" ~/.zshrc 2>/dev/null; then
        cat >> ~/.zshrc << EOF

# Auto-start minishell
if [[ \$- == *i* ]] && [[ -z "\$MINISHELL_ACTIVE" ]]; then
    export MINISHELL_ACTIVE=1
    echo "🚀 Starting minishell..."
    echo "💡 Type 'exit' to return to zsh, Ctrl+D to close terminal"
    "$MINISHELL_PROG"
    echo "👋 Exited minishell, back to zsh"
fi
EOF
        echo "✅ Added auto-start minishell to ~/.zshrc"
    else
        echo "ℹ️  Auto-start minishell already configured in ~/.zshrc"
    fi
fi

echo "✅ Setup complete! Your minishell will start automatically."
echo "💡 The environment variable MINISHELL_ACTIVE prevents infinite loops."
echo "🔄 Open a new terminal or run: exec \$SHELL"