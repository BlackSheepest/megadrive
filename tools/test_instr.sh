#!/bin/bash

# Codes couleur ANSI
GREEN='\033[32m'
ORANGE='\033[38;5;208m'
RESET='\033[0m'

# Initialiser les compteurs
passed=0
failed=0

# Charger les tests depuis le fichier
TESTS_FILE="$(dirname "$0")/instr.tst"
mapfile -t tests < "$TESTS_FILE"

printf "   | %-20s | %-40s | %s\n" "PARAM" "VERIF" "RESULTAT"
echo   "   -------------------------------------------------------------------------------"

for test in "${tests[@]}"; do
    IFS='|' read -r check param <<< "$test"
    result=$(./instr $param | grep Resultat | cut -d ' ' -f 2-)
    check="${check^^}"
    
    # Formater le param en 4 colonnes de 4 caractères hex
    formatted_param=$(echo "$param" | tr ' ' '\n' | awk '{printf "%-4s ", $0}' | sed 's/ $//')
    
    if [[ "$result" == "$check" ]]; then
        printf "${GREEN} ✓ | %-20s | %-40s | %s${RESET}\n" "$formatted_param" "$check" "$result"
        ((passed++))
    else
        printf "${ORANGE} ✗ | %-20s | %-40s | %s${RESET}\n" "$formatted_param" "$check" "$result"
        ((failed++))
    fi
done

# Afficher le résumé
echo ""
echo "====== RÉSUMÉ ======"
total=$((passed + failed))
if [[ $total -gt 0 ]]; then
    percent_passed=$((passed * 100 / total))
    percent_failed=$((100 - (passed * 100 / total)))
    echo "Réussis : $passed ($percent_passed%)"
    echo "Échoués : $failed ($percent_failed%)"
else
    echo "Réussis : $passed"
    echo "Échoués : $failed"
fi
echo "Total : $total"
echo "===================="