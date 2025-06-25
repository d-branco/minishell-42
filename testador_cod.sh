#!/bin/bash

# === CONFIGURAÇÃO INICIAL ===
MINISHELL_PATH="./minishell"
COMANDOS_FILE="comandos.txt"
BASH_OUT="bash_out.txt"
MSH_OUT="ms_out.txt"
RELATORIO="relatorio.txt"

# === CORES ===
GREEN="\033[0;32m"
RED="\033[0;31m"
YELLOW="\033[0;33m"
BLUE="\033[0;34m"
BOLD="\033[1m"
RESET="\033[0m"

# === FLAGS ===
STRICT=true
STOP_ON_FAIL=false
VERBOSE=false
COLOR=true

# === PARSE DE FLAGS ===
for arg in "$@"; do
	case $arg in
		--strict) STRICT=true ;;
		--ignore-whitespace) STRICT=false ;;
		--stop-on-fail) STOP_ON_FAIL=true ;;
		--verbose) VERBOSE=true ;;
		--no-color) COLOR=false ;;
		--help)
			echo "Uso: ./testador_cod.sh [flags]"
			echo "Flags disponíveis:"
			echo "  --strict              Comparação rigorosa (default)"
			echo "  --ignore-whitespace   Ignora espaços e linhas vazias"
			echo "  --stop-on-fail        Interrompe ao primeiro erro"
			echo "  --verbose             Mostra saída mesmo quando correta"
			echo "  --no-color            Desativa coloração"
			echo "  --help                Mostra esta ajuda"
			exit 0
			;;
		*)
			echo "Flag desconhecida: $arg"
			exit 1
			;;
	esac
done

# === REMOVE COR SE PEDIDO ===
if [ "$COLOR" = false ]; then
	GREEN=""; RED=""; YELLOW=""; BLUE=""; BOLD=""; RESET=""
fi

# === TESTES ===
> "$RELATORIO"
echo -e "${BOLD}${BLUE}🧪 Iniciando testes...${RESET}"

if [ ! -f "$COMANDOS_FILE" ]; then
	echo -e "${RED}Arquivo $COMANDOS_FILE não encontrado.${RESET}"
	exit 1
fi

diff_cmd="diff"
if [ "$STRICT" = false ]; then
	diff_cmd="diff -B -w -Z"
fi

i=1
cmd_block=""

while IFS= read -r line || [ -n "$line" ]; do
	if [[ "$line" == "===" ]]; then
		if [[ -n "$cmd_block" ]]; then
			echo -e "${YELLOW}🔹 Teste $i:${RESET}"
			echo "$cmd_block"

			# Executa no bash
			bash -c "$cmd_block" > "$BASH_OUT" 2>&1
			RET_BASH=$?

			# Executa no minishell
			echo "$cmd_block" | $MINISHELL_PATH > "$MSH_OUT" 2>&1
			RET_MSH=$?

			if $diff_cmd "$BASH_OUT" "$MSH_OUT" > /dev/null && [ "$RET_BASH" -eq "$RET_MSH" ]; then
				echo -e "${GREEN}✅ OK${RESET}"
				echo "🔹 Teste $i:" >> "$RELATORIO"
				echo "$cmd_block" >> "$RELATORIO"
				echo "✅ OK" >> "$RELATORIO"
				[ "$VERBOSE" = true ] && {
					echo "--- Saída do bash:"
					cat "$BASH_OUT"
					echo "--- Saída do minishell:"
					cat "$MSH_OUT"
				}
			else
				echo -e "${RED}❌ Diferença encontrada${RESET}"
				echo "🔹 Teste $i:" >> "$RELATORIO"
				echo "$cmd_block" >> "$RELATORIO"
				echo "❌ Diferença encontrada:" >> "$RELATORIO"

				if ! $diff_cmd "$BASH_OUT" "$MSH_OUT" > /dev/null; then
					echo "--- Saída do bash:" >> "$RELATORIO"
					cat "$BASH_OUT" >> "$RELATORIO"
					echo "--- Saída do minishell:" >> "$RELATORIO"
					cat "$MSH_OUT" >> "$RELATORIO"
				fi

				if [ "$RET_BASH" -ne "$RET_MSH" ]; then
					echo "--- Código de saída bash: $RET_BASH" >> "$RELATORIO"
					echo "--- Código de saída minishell: $RET_MSH" >> "$RELATORIO"
				fi

				if [ "$STOP_ON_FAIL" = true ]; then
					echo -e "${RED}Parando no erro.${RESET}"
					exit 1
				fi
			fi

			echo "" >> "$RELATORIO"
			cmd_block=""
			((i++))
		fi
	else
		cmd_block+="$line"$'\n'
	fi
done < "$COMANDOS_FILE"
