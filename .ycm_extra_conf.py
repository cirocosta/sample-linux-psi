defaultFlags = [
        '-I/usr/include',
        '-std=gnu99', # this allows us to leverage `asm`
        '-xc'
]

def FlagsForFile( filename, **kwargs ):
        return { 'flags': defaultFlags }
