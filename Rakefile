desc "Build Env"
task :build_env do
  system('git submodule update --init --recursive')
  system('build/auto-build-env')
end

desc "Debug Bison Output"
task :debug_grammar do
  system('less src/y.tab.output')
end
task :dg => :debug_grammar

# Default Rake Task
task :default => :build_env
