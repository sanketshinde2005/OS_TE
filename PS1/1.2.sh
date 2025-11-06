FILE="phonebook.txt"
if [ ! -f "$FILE" ]; then
    touch "$FILE"
fi
add_entry() {
    echo "Enter First Name:"
    read fname
    echo "Enter Last Name:"
    read lname
    echo "Enter Phone Number:"
    read phone
    if grep -q -i "$fname[[:space:]]\+$lname[[:space:]]\+$phone" "$FILE"; then
        echo "⚠️ Entry already exists!"
    else
        echo -e "${fname}\t${lname}\t${phone}" >> "$FILE"
        echo "✅ Entry added successfully!"
    fi
}
search_entry() {
    echo "Enter Name or Phone Number to search:"
    read key
    result=$(grep -i "$key" "$FILE")
    if [ -z "$result" ]; then
        echo "❌ No match found."
    else
        echo -e "Matching entries:"
        echo -e "------------------"
        echo -e "$result" | column -t
    fi
}
sort_phonebook() {
    echo "📖 Sorting phonebook by Last Name..."
    sort -k2,2 "$FILE" -o "$FILE"
    echo "✅ Sorted successfully! Use any text editor or 'cat $FILE' to view."
}
delete_entry() {
    echo "Enter First Name of the entry to delete:"
    read fname
    echo "Enter Last Name of the entry to delete:"
    read lname

    if grep -q -i "$fname[[:space:]]\+$lname" "$FILE"; then
        grep -vi "$fname[[:space:]]\+$lname" "$FILE" > temp.txt && mv temp.txt "$FILE"
        echo "🗑️ Entry deleted successfully!"
    else
        echo "❌ Entry not found."
    fi
}
while true; do
    echo ""
    echo "========= PHONEBOOK ========="
    echo "1. Add New Entry"
    echo "2. Search Entry"
    echo "3. Sort by Last Name"
    echo "4. Delete Entry"
    echo "5. Quit"
    echo "=============================="
    echo -n "Enter your choice [1-5]: "
    read choice

    case $choice in
        1) add_entry ;;
        2) search_entry ;;
        3) sort_phonebook ;;
        4) delete_entry ;;
        5) echo "👋 Exiting Phonebook. Bye!"; exit 0 ;;
        *) echo "Invalid choice! Enter 1–5." ;;
    esac
done
