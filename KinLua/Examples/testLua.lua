--
-- User: Kin
-- Date: 2018/9/22 0022
-- Time: 14:53
-- Copyright © 2018 jihuisoft. All rights reserved.
--

print("hello World, i'm in testLua.lua")


function ptFibona(nums)
    print("斐波拉序列:")

    local firstNumber = 1
    local secondNumber = 1

    print(firstNumber)

    for i= 1,nums do
        firstNumber,secondNumber = secondNumber, firstNumber + secondNumber
        print(firstNumber)

    end

end

function callTest(a,b)
    print("compute a + b")
    return a + b, 26, 31, 0, 2
end

