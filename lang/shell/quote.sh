func() {
    echo $1
    echo $2
}

par="abc def"
func $par

func "$par"
