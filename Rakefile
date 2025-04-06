# --------------------------------------------- #
# Run Rakefile from the root folder:            #
#                                               #
# `rake` - Default build                        #
# `rake d` - Build with debugging tools         #
# --------------------------------------------- #

CFLAGS = "-Wall -O3 -pthread"
TARGET = "build/test.exe"
SRC = "src/main.c src/glad/glad.c"

#-------------------------------------#
#             Main scripts            #
#-------------------------------------#

# Default full build settings
task :default => [:notify, :disable_debug, :build, :run]

# Full build settings + extra debugging tools
task :d => [:notify, :enable_debug, :build, :run]

#-------------------------------------#
#          Atomic procedures          #
#-------------------------------------#

task :notify do
  puts "-----Pixel fog test-----"
end

# Debugging tools on
task :enable_debug do
  sh "ruby script/enable_debug.rb", verbose: false
end

# Debugging tools off
task :disable_debug do
  sh "ruby script/disable_debug.rb", verbose: false
end

# Compile the game
task :build do
  puts "| Building..."
  sh "gcc #{SRC} -o #{TARGET} #{CFLAGS} -I include/ -L lib/ -lglfw3 -lopengl32 -lgdi32", verbose: false
end

# Run game exe
task :run do
  puts "| Running..."
  sh "start #{TARGET}", verbose: false
end