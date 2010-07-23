#!/usr/bin/env ruby
#
# programme de demo :
# - generant des fichiers de test avec pack
# - lisant ces memes fichiers avec unpack
# - syntaxe equivalente en KOPT
# - test du module ruby KOPT
#

puts "**** PACK/UNPACK VERSUS KOPT"
puts "**** JUST A SIMPLE DEMO OF PACKING/UNPACKING DATA"

#### tools
ROW_SIZE = 16
def dump_file(fn)
  row = 0
  File.new(fn, "r+").each_byte do |byte|
    printf("%02X ", byte)
    if (!(row += 1 % 16))
      printf("\n") 
      row = 0
    end
  end  
end

def to_bitvector(val)
  bv = []
  (0..7).each do |x|
    bv << "#{(((1 << x) & val) >> x)}"
  end
  return bv
end

################################ TESTS ################################
# 01
f = File.new("dump_01.bin", "w+")
f.binmode
d = to_bitvector(0xae).pack("b8") # 00001111
f.write(d)
f.close

# 01a
fa = File.new("dump_01.bin", "r+")
fa.each_byte do |x|
  d = x.to_s.unpack("b*") # 00001111
end
puts "READ #{d}"
fa.close

# 02
f = File.new("dump_02.bin", "w+")
f.binmode
d = [0xff].pack("c") # 00001111
f.write(d)
f.close

################################ MAIN  ################################


Dir.glob("dump_*.bin") do |fn|
  puts "**** BEGIN DUMP #{fn}"
  dump_file(fn)
  puts "\n**** END DUMP #{fn}\n"
end
