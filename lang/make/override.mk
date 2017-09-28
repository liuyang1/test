d=.
target: $(d)/target
	echo "target"

$(d)/target:
	echo "./target"

target_b: ./target_b
	echo "target_b"

./target_b:
	echo "./target_b"
