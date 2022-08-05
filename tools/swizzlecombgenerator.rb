
elementname = ['x', 'y', 'z', 'w']

r = ""

(1..ARGV[0].to_i).each{|vsize| 
velement = ARGV[1].to_i
vcomb = [0] * vsize;

while true
    str = "SwizzleProxy#{vsize}<T, #{ARGV[0].to_i}, "
    vcomb.each{|n|
        str += "#{n}, "
    }

    #Write in file
    str = str[0...-2] + "> "

    vcomb.each{|n|
        str += "#{elementname[n]}"
    }

    str += ";\n"
    r += str
    
    vcomb[0]+=1;

    for i in (0...(vsize - 1))
        if vcomb[i] >= velement
            vcomb[i] = 0;
            vcomb[i+1]+=1;
        end
    end
    if vcomb[vsize - 1] >= velement
        break
    end
end
}

File.open("out.txt", "w") {|f| f.write(r)}