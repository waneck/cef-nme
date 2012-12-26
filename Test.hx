class Test
{
	static function main()
	{
		trace("here");
		cpp.Lib.load("hxcef", "cef_init",0)();
	}
}