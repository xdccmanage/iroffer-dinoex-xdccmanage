
# htdocs/json.txt.rb: export file info as list.
#   pack:  "/info.txt.rb?pack=21"
#   range: "/info.txt.rb?pack=21;last=29"
#   group: "/info.txt.rb?group=ZZZ"

#p.k[0] = {b:"XDCC|SHFS|Ayanami", n:1, s:170, f:"[PiaNex+SHFS]Moonlight_Lady_01V2_(640x480).[76E333AF].rar"};
#p.k[1] = {b:"XDCC|SHFS|Ayanami", n:2, s:171, f:"[PiaNex+SHFS]Moonlight_Lady_02_(640x480).rar"};
#p.k[2] = {b:"XDCC|SHFS|Ayanami", n:3, s:171, f:"[PiaNex+SHFS]Moonlight_Lady_03_(640x480).rar"};

now = Time.now.to_s

bot = IrofferEvent.new
version = bot.irconfig( "version" )

puts "// Generated by #{version} on #{now}\r"

def print_head( bot, pack )
  bytes = bot.info_pack(pack, "bytes" )
  if bytes.nil?
    return nil
  end
  desc = bot.info_pack(pack, "desc" )
  bytes = bot.info_pack(pack, "bytes" )
  mtime = bot.info_pack(pack, "mtime" )
  mega = bytes / 1024 / 1024
  printf( 'p.k[%d] = {b:"%s", n:%d, s=%d f:"%s"};%s', $count, bot.mynick, pack, mega + 1,  desc, "\n" )
  $count += 1
  return pack + 1
end

$count = 0
pack = 1
while true do
  pack = print_head( bot, pack )
  if pack.nil?
    break
  end
end

# eof