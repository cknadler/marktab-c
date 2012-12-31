desc "Build Env"
task :build_env do
  system('./auto-build-env')
end

desc "Check Bison Output"
task :debug_grammar do
  system('less src/y.tab.output')
end
task :dg => :debug_grammar

# Default Rake Task
task :default => :build_env
