#-------------------------------------------------------------------------------------------------#
#                                                                                                 #
#-------------------------------------------------------------------------------------------------#

def annotate(*args, **kargs):

    """Decorator to set a function's __annotations__ like Py3"""

    from inspect import getargspec

    def decorate(func):
        try:
            func.__annotations__
        except AttributeError:
            argspec = getargspec(func)
            func.__annotations__ = dict(zip(argspec.args, args), **kargs)
            if "returns" in kargs:
                func.__annotations__["return"] = func.__annotations__.pop("returns")
        return func

    return decorate

#---------------------------------------------  EOF  ---------------------------------------------#
