import multiprocessing 

def fibonacci(n):
  a=0
  b=1
  if n==0:
    return n    
  elif n==1:
    return n    
  else: 
    for i in range(2, n):
      c=a+b
      a=b
      b=c
    return b

if __name__=='__main__':
  pool = multiprocessing.Pool()
  procesos = [pool.apply_async(fibonacci, args=(x,)) for x in range(1000)]
  resultado = [proceso.get() for proceso in procesos]
  print(resultado)