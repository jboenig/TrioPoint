
// One might be given to ask, "What the heck is this??"

// Well, the deal is this:  Custom-AppWizards, like DocJet,
//   can only construct projects that are like the existing
//   project type "Win32 Applications", there's no way to
//   make one based on a makefile.  If you don't have this
//   file, the generator will actually still run, because
//   the rule that causes it to run is a custom make rule,
//   and those fire even if the executable fails to build.
//
// That's actually the way DocJet used to work, which was fine
//   except that you'd always get these bogus warning messages
//   about the fact that there are no source files.  In the
//   end, having this bogus target is less bad than the constant
//   appearance of the bogus error message.

int __stdcall WinMain( void *, void *, char *, int )
{
	return(0);
}
