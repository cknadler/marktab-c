#!/usr/bin/env ruby

dependencies = 
  "aclocal",
  "autoconf",
  "automake",
  "autoreconf"

commands =
  "aclocal",
  "autoconf",
  "automake --add-missing",
  "autoreconf",
  "automake",
  "./configure"

# ASCII text coloring
def purpify(text)
  "\033[1;35m" + text + "\033[0m"
end

def greenify(text)
  "\033[0;32m" + text + "\033[0m"
end

def redify(text)
  "\033[0;31m" + text + "\033[0m"
end

# Check dependencies
missing_dependencies = Array.new

dependencies.each do |dependency|
  puts "#{purpify "build-env:"} #{greenify "checking #{dependency}"}"
  if not system("which #{dependency}", :out => "/dev/null")
    missing_dependencies.push dependency
  end
end

if not missing_dependencies.empty?
  puts redify "build-env failed..."

  missing_dependencies.each do |missing|
    puts redify "Missing #{missing}"
  end

  puts redify "Install the missing dependencies and run again."
  exit 1
end

# Execute build commands
commands.each do |command|
  puts "#{purpify "build-env:"} #{greenify "#{command}"}"
  system("#{command}")
end

puts greenify "\nYour environment setup is complete. Run `make` to build hkl.\n"

