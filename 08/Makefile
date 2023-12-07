all : cjson

cjson:
	sudo python3 setup.py install
	python3 -m pip install ujson faker
	python3 test.py
	python3 stress_test.py