Follow steps on `libtroch page <https://pytorch.org/cppdocs/installing.html>`_ for running the program

Seralizing and Deseralizing
============================

Initially, I did not know how the tensor was arranged in-memory.
Due to this, I went ahead with seralizing and deseralizing using the built-in torch methods.
The seralizing method i.e. torch::save would save the tensor into a string stream and then we would have to get the bytes from the output stream into a buffer.
The deseralizing method i.e. torch::load would load the tensor from string stream and we would have to load the string stream with the buffer.