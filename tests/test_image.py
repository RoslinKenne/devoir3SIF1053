import pathlib,subprocess,tempfile,unittest
ROOT=pathlib.Path(__file__).resolve().parents[1]
class ImageTests(unittest.TestCase):
 def test_cli(self):
  with tempfile.TemporaryDirectory() as d:
   d=pathlib.Path(d);binary=d/'image'
   subprocess.run(['gcc','-std=c11','-Wall','-Wextra','-Werror',str(ROOT/'main.c'),'-o',str(binary)],check=True)
   source=d/'a.raw';out=d/'b.raw';back=d/'c.raw';source.write_bytes(bytes(range(256)))
   def run(*args):return subprocess.run([str(binary),*map(str,args)],capture_output=True)
   self.assertEqual(run(source,out,16,16,42).returncode,0)
   self.assertEqual(run(out,back,16,16,42).returncode,0);self.assertEqual(source.read_bytes(),back.read_bytes())
   for args in [(source,out,0,16,42),(source,out,16,16,256),(source,out,15,16,42),(d/'missing',out,16,16,42),(source,source,16,16,42),(source,d/'missing'/'out',16,16,42),(source,out,'999999999999999999999',16,42)]:
    self.assertNotEqual(run(*args).returncode,0)
if __name__=='__main__':unittest.main()
