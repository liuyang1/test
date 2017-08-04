a: b
	@echo "a depend on b"

# Make will audo drop when it detect circular dependency.
b: a
	@echo "b depend on a"
