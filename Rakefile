gem 'rake-tasks'

require 'rake'
require 'rake/autotoolstask'

task :default => :pkgs
task :cruise => [:clean, :pkgs, :sign, :install]
task :pkgs => :pkg 

desc 'Build RPM packages'
PkgTask = Rake::AutoToolsRPMTask.new( :pkg => File.join( File.dirname(__FILE__), 'libFinagle.spec' ) )
CLEAN.include File.join( File.dirname(__FILE__), 'configure' )

desc 'Sign packages'
task :sign => :pkg  do |t|
  rpm_paths = PkgTask.spec.packages.map { |p|  p.path }
  system( "sudo rpm --resign #{rpm_paths.join ' '}" ) and $?.exitstatus.zero?  or raise "Package signing failed"
end

desc 'Install packages'
task :install => :pkg  do |t|
  rpm_paths = PkgTask.spec.packages.map { |p|  p.path }
  system( "sudo rpm -Uvh --oldpackage #{rpm_paths.join ' '}" ) and $?.exitstatus.zero?  or raise "Package install failed"
end

#RPMDIR='/var/www/html/rpms'
#repomanage --keep 1 --old --space $RPMDIR | xargs rm -f  &&
#createrepo $RPMDIR  &&

