# analyzeArgs.py
from argparse import ArgumentParser

# See https://stackoverflow.com/questions/26785952/python-argparse-as-a-function

def getArgs(argv=None):
# Set command line configurable parameters. Do python3 program.py -h to see this in action.
    parser = ArgumentParser(description="Run analyze program")
    parser.add_argument("-n", "--nevsToRead", type=int, default=-1, help="Number of events to read - defaults to all")
    parser.add_argument("-w", "--which", type=str, default="Sleptons-LSP260", help="LHE data-set filename (no .lhe) (str)")
    parser.add_argument("-t", "--target", type=float, default=1.0, help="Target integrated luminosity [inv fb]")
    parser.add_argument("-l", "--lumi", type=float, default=1.0,   help="Actual simulated integrated luminosity [inv fb]")  
    parser.add_argument("-p", "--prefix", type=str, default=".",   help="LHE file path prefix (str)")       
    
    args=parser.parse_args(argv)
    print('(pvalueArgs.getArgs     ) Found argument list: ',args)
    
    return args
    
def showArgs(nevsToRead, which, target, lumi, prefix):
# Check these are what we want
    print('(analyzeArgs.ShowArgs    ) Program has set')
    print('which:            ',which)
    print('target lumi [/fb] ',target)
    print('actual lumi [/fb] ',lumi)
    print('# of events to read ',nevsToRead)
    print('prefix ',prefix)
    print('Note actual lumi may need to be corrected if not all events are to be read ...')
    return
        
def getArguments(argv=None):
# Do 2 things at once.
# i)   set defaults and parse them using getArgs above
# ii)  set values for our program

    args = getArgs(argv)

    print('(analyzeArgs.getArguments) Assigning arguments to program variables')
    
    nevsToRead = args.nevsToRead
    which = args.which
    target = args.target
    lumi = args.lumi
    prefix = args.prefiz
   
    return nevsToRead,which,target,lumi,prefix 
