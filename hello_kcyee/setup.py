import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="hello_kcyee", # Replace with your own username
    version="0.0.5",
    author="Kwong Cheong, Yee",
    author_email="yee.kwong.cheong@gmail.com",
    description="Print Hello",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/tensorfactory/PiFaceCam",
    packages=["hello_kcyee"],
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: Freeware",
        "Operating System :: POSIX :: Linux",
    ],
    include_package_data=True,
    python_requires='>=3.5',
)