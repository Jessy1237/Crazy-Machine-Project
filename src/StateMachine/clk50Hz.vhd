library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
 
entity clk50Hz is
    Port (
        clk    : in  STD_LOGIC;
        reset  : in  STD_LOGIC;
        clk_out: out STD_LOGIC
    );
end clk50Hz;
 
architecture Behavioral of clk50Hz is
    signal temporal: STD_LOGIC;
    signal counter : integer range 0 to 999999 := 0;
begin
    freq_divider: process (reset, clk) begin
        if (reset = '1') then
            temporal <= '0';
            counter  <= 0;
        elsif rising_edge(clk) then
            if (counter = 999999) then
                temporal <= NOT(temporal);
                counter  <= 0;
            else
                counter <= counter + 1;
            end if;
        end if;
    end process;
 
    clk_out <= temporal;
end Behavioral;