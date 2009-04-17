gem 'rake-tasks'

require 'rake'
require 'rake/autotoolstask'

task :default => :pkgs
task :cruise => [:clean, :pkgs, :sign, :install]
task :pkgs => :pkg 

desc 'Build RPM packages'
PkgTask = Rake::AutoToolsRPMTask.new( :pkg => File.join( File.dirname(__FILE__), 'libFinagle.spec' ) )
CLEAN.include File.join( File.dirname(__FILE__), 'configure' )

desc 'Sign all packages'
task :sign => :pkgs  do |t|
  pkgs = PkgTask.spec.packages.reject { |pkg|  RPM::Package.new( pkg.path ).has_sig? }
  next if pkgs.empty?

  pkgs = pkgs.collect { |p| p.path }.flatten.map { |p| "\"#{p}\"" }.join(' ')
  system( "rpm --resign #{pkgs}" ) or
    raise "Package signing failed"
end

desc 'Install packages'
task :install => :pkgs  do |t|
  rpm_paths = PkgTask.spec.packages.map { |p|  p.path }
  system( "sudo rpm -Uvh --oldpackage #{rpm_paths.join ' '}" ) and $?.exitstatus.zero?  or raise "Package install failed"
end

#RPMDIR='/var/www/html/rpms'
#repomanage --keep 1 --old --space $RPMDIR | xargs rm -f  &&
#createrepo $RPMDIR  &&

