# mkdir venv
# python3 -m venv myvenv
# myvenv/bin/pip3 install networkx
# myvenv/bin/pip3 install numpy matplotlib
# myvenv/bin/pip3 install pygraphviz
# myvenv/bin/python -m pip install \
#     --config-settings=build_ext \
#     --config-settings="-I/opt/homebrew/opt/graphviz/include/" \
#     --config-settings="-L/opt/homebrew/opt/graphviz/lib/" \
#     pygraphviz

myvenv/bin/pip install --config-settings="--global-option=build_ext" \
    --config-settings="--global-option=-I$(brew --prefix graphviz)/include/" \
    --config-settings="--global-option=-L$(brew --prefix graphviz)/lib/" \
    pygraphviz
    #--global-option="-I$(brew --prefix graphviz)/include/" --global-option="-L$(brew --prefix graphviz)/lib/" 
