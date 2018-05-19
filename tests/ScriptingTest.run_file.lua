--
-- Created by IntelliJ IDEA.
-- User: sandy
-- Date: 18/05/18
-- Time: 10:33 PM
--

function max(num1, num2)

    if (num1 > num2) then
        result = num1;
    else
        result = num2;
    end

    return result;
end

-- calling a function
print(max(10, 4))
print(max(5, 6))

