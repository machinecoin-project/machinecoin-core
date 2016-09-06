from distutils.core import setup
setup(name='macspendfrom',
      version='1.0',
      description='Command-line utility for machinecoin "coin control"',
      author='Gavin Andresen',
      author_email='gavin@machinecoinfoundation.org',
      requires=['jsonrpc'],
      scripts=['spendfrom.py'],
      )
