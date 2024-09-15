module led(a_in, b_in, c_in, sum, carry);
    input a_in, b_in, c_in;
    output reg sum, carry;

    always@(b_in)

begin

    sum = a_in ^ b_in ^ c_in;
    carry = (a_in & b_in) | (b_in & c_in) | (c_in & a_in);
end


endmodule