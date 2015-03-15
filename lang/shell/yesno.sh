#! /usr/bin/env bash
while true; do
    read -p "Do you wish to install this program? must startswith y/n" yn
    case $yn in
        [Yy]* ) echo "yes"; break;;
        [Nn]* ) exit;;
        * ) echo "Please answer yes or no.";;
    esac
done

echo "Do you wish to install this program?"
echo "must select 1 or 2"
select yn in "Yes" "No"; do
    case $yn in
        Yes ) echo "yes"; break;;
        No ) exit;;
    esac
done
