#-------------------------------------------------------------------------------------------------#
#                                                                                                 #
#-------------------------------------------------------------------------------------------------#

__all__ = ('__title__','__summary__','__uri__',
           '__version_info__','__version__','__author__','__email__','__copyright__','__license__')

__title__        = "annotate"
__summary__      = "Decorator to set a function's __annotations__ like Py3"
__uri__          = "http://pypi.python.org/pypi/annotate/"
__version_info__ = type("version_info",(), dict(major=0,minor=3,micro=3,releaselevel="beta",serial=0))
__version__      = "%d.%d.%d" % (__version_info__.major,__version_info__.minor,__version_info__.micro)
__author__       = "Adam Karpierz"
__email__        = "python@python.pl"
__copyright__    = "Copyright (c) 2014 %s, All Rights Reserved" % __author__
__license__      = "zlib/libpng License ; http://opensource.org/licenses/zlib"

#---------------------------------------------  EOF  ---------------------------------------------#
