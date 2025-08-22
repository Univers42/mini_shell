#!/bin/bash
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    assert.sh                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/22 14:26:44 by dlesieur          #+#    #+#              #
#    Updated: 2025/08/22 15:25:35 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

generate_minishell_script()
{
    cat <<EOF
echo hello from minishell
ls
EOF
}

run_bash_script()
{
    generate_minishell_script > bash_script.txt
    bash < bash_script.txt > bash_all_output.txt 2>&1
}

run_minishell()
{
    local find_mini
    find_mini=$(find ../../  -maxdepth 3 -name "minishell" -type f -executable | head -n 1)
    if [ -n "$find_mini" ] && [ -x "$find_mini" ]; then
        generate_minishell_script | script -q -c "$find_mini" > minishell_all_output.txt
    else
        echo "minishell is not found"
        exit 1
    fi
}

compare_outputs()
{
    echo "Diff between Bash and Minishell outputs:"
    diff -u bash_all_output.txt minishell_all_output.txt
}

main()
{
    run_bash_script
    run_minishell
    compare_outputs
}

main