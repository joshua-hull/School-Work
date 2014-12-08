bipush(-40)     comment(`Load starting Celcius temperature.')
istore(1)

label(loop)

iload_1
bipush(120)

isub            
ifgt(done)      comment(`Check to make sure we haven't increment too far yet.')

iload_1
bipush(9)
imul            comment(`multiply by nine since we only get integers.')

bipush(5)       comment(`divide by 5')
idiv

bipush(32)      comment(`add 32')
iadd

istore_2        comment(`store to variable 2 for Fahrenheit')

bipush(2)
bipush(1)

invokevirtual(1)    comment(`print temperatures')
invokevirtual(2)

iload_1             comment(`add ten degrees to the celcius')
bipush(10)
iadd
istore_1

goto(loop)

label(done)

return
