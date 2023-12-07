from setuptools import Extension, setup


def main():
    setup(
        name="cjson",
        author="Ivan Sidorov",
        version="0.0.1",
        ext_modules=[Extension("cjson", ["cjson.c"])],
    )


if __name__ == "__main__":
    main()
