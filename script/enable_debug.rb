file_path = File.join(File.dirname(__FILE__), '../src/common.h')

file = File.open(file_path, "r")
contents = file.read()
file.close()

newContents = contents.gsub("#define DEBUG_FLAG 0", "#define DEBUG_FLAG 1")

file = File.open(file_path, "w")
file.write(newContents)
file.close()

puts "| DEBUG : 1"